/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Kalray SA. Feb 2022. All rights reserved.
 * NB: Some comments added from the original Solaris release
 * and/or license-compatible forks for clarity.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
#include <hal/cos_bsp.h>
#endif

/* Fraction of text space to allocate for from hash buckets.
 * The value of HASHFRACTION is based on the minimum number of bytes
 * of separation between two subroutine call points in the object code.
 * Given MIN_SUBR_SEPARATION bytes of separation the value of
 * HASHFRACTION is calculated as:
 *
 *  HASHFRACTION = MIN_SUBR_SEPARATION / (2 * sizeof(short) - 1);
 *
 * For the VAX, the shortest two call sequence is:
 *
 *  calls $0,(r0)
 *  calls $0,(r0)
 *
 * which is separated by only three bytes, thus HASHFRACTION is
 * calculated as:
 *
 *  HASHFRACTION = 3 / (2 * 2 - 1) = 1
 *
 * Note that the division above rounds down, thus if MIN_SUBR_FRACTION
 * is less than three, this algorithm will not work!
 *
 * NB: for the kernel we assert that the shortest two call sequence is:
 *
 *  calls $0,_name
 *  calls $0,_name
 *
 * which is separated by seven bytes, thus HASHFRACTION is calculated as:
 *
 *  HASHFRACTION = 7 / (2 * 2 - 1) = 2
 */
#define HISTFRACTION 2
#define HISTCOUNTER  unsigned short
#define HASHFRACTION 2
#define ARCDENSITY   3

#define SCALE_1_TO_1 0x10000L

#define ROUNDDOWN(x, y) (((x) / (y)) * (y))
#define ROUNDUP(x, y)   ((((x) + (y) - 1) / (y)) * (y))

/* GProf "file format" structs */

struct gmon_file_header {
  uintptr_t lowpc;
  uintptr_t highpc;
  unsigned int count;
};

struct gmon_file_arc {
  uintptr_t frompc;
  uintptr_t selfpc;
  unsigned long count;
};

/* Internal monitor state */

typedef unsigned long gmon_arcindex_t;

enum gmon_state {
  GMON_STATE_OFF,
  GMON_STATE_ON,
  GMON_STATE_BUSY,
};

struct gmon_to {
  uintptr_t selfpc;
  unsigned long count;
  gmon_arcindex_t link;
};

struct gmon {
  enum gmon_state state;
  uintptr_t lowpc, highpc;

  /* Sampling */
  unsigned short *samples;
  size_t samplescount;
  unsigned int scale;

  /* Call graph */
  gmon_arcindex_t *froms;
  size_t fromscount;
  struct gmon_to *tos;
  size_t toscount;
};

static __thread struct gmon gmon;

extern __attribute__((weak)) int KVX_GPROF_SAMPLE_TIMER;
extern __attribute__((weak)) int KVX_GPROF_SAMPLE_FREQ;

/* Monitoring functions */
#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
static void
msample(int timer_id, __cos_context_t *context) {
  /* Ref. man pages for the previously defined
   * profil(2), now profil(3) for behaviours details */
  const uintptr_t index = (context->spc - gmon.lowpc) * gmon.scale / 65536;

  if (index < gmon.samplescount) {
    gmon.samples[index]++;
  }
}
#endif

static void
mupdate_call_graph(uintptr_t selfpc, uintptr_t frompc) {
  /*
   * check that frompcindex is a reasonable pc value.
   * for example:  signal catchers get called from the stack,
   * not from text space. too bad.
   */
  frompc -= gmon.lowpc;
  if (frompc > gmon.highpc - gmon.lowpc) {
    return;
  }

  gmon_arcindex_t * const frompcindex
    = gmon.froms + frompc / (HASHFRACTION * sizeof (*gmon.froms));
  gmon_arcindex_t toindex = *frompcindex;

  if (toindex == 0) {
    /* first time traversing this arc. */
    toindex = ++gmon.tos[0].link;

    if (toindex >= gmon.toscount) {
      /* Used to be overflow */
      return;
    }

    *frompcindex = toindex;
    struct gmon_to *top = gmon.tos + toindex;
    top = gmon.tos + toindex;
    top->selfpc = selfpc;
    top->count = 1;
    top->link = 0;

    return;
  }

  struct gmon_to *top = gmon.tos + toindex;
  if (top->selfpc == selfpc) {
    /* arc at front of chain; usual case. */
    top->count++;
    return;
  }
  /*
   * have to go looking down chain for it.
   * top points to what we are looking at,
   * prevtop points to previous top.
   * we know it is not at the head of the chain.
   */
  for (; /* goto done */; ) {
    if (top->link == 0) {
      /*
       * top is end of the chain and none of the chain
       * had top->selfpc == selfpc.
       * so we allocate a new tostruct
       * and link it to the head of the chain.
       */
      toindex = ++gmon.tos[0].link;
      if (toindex >= gmon.toscount) {
        /* Used to be overflow */
        return;
      }

      top = gmon.tos + toindex;
      top->selfpc = selfpc;
      top->count = 1;
      top->link = *frompcindex;
      *frompcindex = toindex;

      return;
    }
    /* otherwise, check the next arc on the chain. */
    struct gmon_to *prevtop = top;
    top = gmon.tos + top->link;
    if (top->selfpc == selfpc) {
      /*
       * there it is.
       * increment its count
       * move it to the head of the chain.
       */
      top->count++;
      toindex = prevtop->link;
      prevtop->link = top->link;
      top->link = *frompcindex;
      *frompcindex = toindex;

      return;
    }
  }
}

/* Monitor state handling */
static void
monstartup(const unsigned char * const lowpc,
  const unsigned char * const highpc) {

  gmon.lowpc = ROUNDDOWN((uintptr_t)lowpc, HISTFRACTION * sizeof (HISTCOUNTER));
  gmon.highpc = ROUNDUP((uintptr_t)highpc, HISTFRACTION * sizeof (HISTCOUNTER));

  const size_t textsize = gmon.highpc - gmon.lowpc;

  gmon.samplescount = textsize / HISTFRACTION;
  gmon.samples = calloc(gmon.samplescount, sizeof (*gmon.samples));
  assert(gmon.samples != NULL);

  gmon.fromscount = textsize / HASHFRACTION;
  gmon.froms = calloc(gmon.fromscount, sizeof (*gmon.froms));
  assert(gmon.froms != NULL);

  gmon.toscount = textsize * ARCDENSITY / 100;
  gmon.tos = calloc(gmon.toscount, sizeof (*gmon.tos));
  assert(gmon.tos != NULL);

  if (gmon.samplescount < textsize) {
    gmon.scale = (float)gmon.samplescount / textsize * SCALE_1_TO_1;
  } else { /* gmon.samplescount == textsize */
    gmon.scale = SCALE_1_TO_1;
  }

  /* Default to timer 0, default sample rate at 0.01 second */
  const uintptr_t sample_timer = (uintptr_t)&KVX_GPROF_SAMPLE_TIMER;
  uintptr_t sample_freq = (uintptr_t)&KVX_GPROF_SAMPLE_FREQ;
  if (!sample_freq) {
    sample_freq = 100;
  }

#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
  mppa_cos_timer_create(sample_timer, msample);
  mppa_cos_timer_set_time(sample_timer, __cos_get_frequency() / sample_freq);
#endif
}

static const char *
mfilename(void) {
  extern char MPPA_COS_NB_CC __attribute__((weak));
  extern char MPPA_COS_NB_CORES_LOG2 __attribute__((weak));

  if ((uintptr_t)&MPPA_COS_NB_CC > 1
    || (uintptr_t)&MPPA_COS_NB_CORES_LOG2 > 0) {
    /* No need for a thread-local buffer here, as all mcleanup()s
     * will be called on the same thread sequentially during exit(). */
    static char name[16];

#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
    if (snprintf(name, sizeof (name), "gmon.%u.%.2u.out",
      (unsigned int)__cos_get_cluster_id(),
      (unsigned int)__cos_get_cpu_id()) >= sizeof (name)) {
      abort();
    }
#endif
    return name;
  } else {
    return "gmon.out";
  }
}

static void
mcleanup() {
#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
  mppa_cos_timer_destroy((uintptr_t)&KVX_GPROF_SAMPLE_TIMER);
#endif
  const char * const filename = mfilename();
  FILE * const out = fopen(filename, "w");
  if (out == NULL) {
    fprintf(stderr, "[mcleanup] error: fopen '%s': %s\n",
      filename, strerror(errno));
    abort();
  }

  /* Writing header */
  const struct gmon_file_header header = {
    .lowpc = gmon.lowpc,
    .highpc = gmon.highpc,
    .count = sizeof (header) + gmon.samplescount,
  };

  if (fwrite(&header, sizeof (header), 1, out) != 1) {
    fprintf(stderr, "[mcleanup] error: fwrite header: %s\n",
      strerror(errno));
    abort();
  }

  /* Writing samples */
  if (fwrite(gmon.samples,
    sizeof (*gmon.samples),
    gmon.samplescount, out) != gmon.samplescount) {
    fprintf(stderr, "[mcleanup] error: fwrite samples: %s\n",
      strerror(errno));
    abort();
  }

  /* Writing call graph */
  for (unsigned int i = 0; i < gmon.fromscount; i++) {
    const uintptr_t frompc
      = gmon.lowpc + i * HASHFRACTION * sizeof (*gmon.froms);

    for (gmon_arcindex_t fromindex = gmon.froms[i];
      fromindex != 0;
      fromindex = gmon.tos[fromindex].link) {
      const struct gmon_file_arc arc = {
        .frompc = frompc,
        .selfpc = gmon.tos[fromindex].selfpc,
        .count = gmon.tos[fromindex].count,
      };

      if (fwrite(&arc, sizeof (arc), 1, out) != 1) {
        fprintf(stderr, "[mcleanup] error: fwrite arc: %s\n",
          strerror(errno));
        abort();
      }
    }
  }

  fclose(out);
}

void
__mcount_internal(uintptr_t selfpc, uintptr_t frompc) {
  switch (gmon.state) {
  case GMON_STATE_OFF:
    gmon.state = GMON_STATE_ON;
    /* _text_start and _text_end are defined within mppa_cos.h */
    monstartup(_text_start, _text_end);
    atexit(mcleanup);
    /* fallthrough */
  case GMON_STATE_ON:
    gmon.state = GMON_STATE_BUSY;
    mupdate_call_graph(selfpc, frompc);
    gmon.state = GMON_STATE_ON;
    break;
  }
}
