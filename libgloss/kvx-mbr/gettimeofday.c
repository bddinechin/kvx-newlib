/*
 * Copyright 2019
 * Kalray Inc. All rights reserved.
 *
 * This software is furnished under license and may be used and copied only
 * in accordance with the following terms and conditions.  Subject to these
 * conditions, you may download, copy, install, use, modify and distribute
 * modified or unmodified copies of this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce and
 *    retain this copyright notice and list of conditions as they appear in
 *    the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Kalray Inc.  The "Kalray Inc" name may not be
 *    used to endorse or promote products derived from this software
 *    without the prior written permission of Kalray Inc.
 *
 * 3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 *    NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL KALRAY BE LIABLE
 *    FOR ANY DAMAGES WHATSOEVER, AND IN PARTICULAR, KALRAY SHALL NOT BE
 *    LIABLE FOR DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *    OR OTHERWISE), EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "errno.h"
#include <sys/time.h>
#include <time.h>

#include <mppa_bare_runtime/kvx/timestamp.h>
#include <mppa_bare_runtime/kvx/cpu.h>

#define NANOSEC_PER_SECOND (1000000000uLL)

int __kvx_getnanotime(struct timespec *tval)
{
  uint64_t ticks = 0;
  uint64_t cycles;
  uint64_t end;

  cycles  = __kvx_cluster_timestamp();
  tval->tv_sec = cycles / _KVX_TIMESTAMP_FREQ;
  tval->tv_nsec = (cycles -  _KVX_TIMESTAMP_FREQ*(uint64_t)tval->tv_sec) * (NANOSEC_PER_SECOND/(float)_KVX_TIMESTAMP_FREQ);

  return 0;
}

int _gettimeofday(struct timeval *tval, void *tz)
{
  struct timespec tp;
  if(!tval){
    errno = EINVAL;
    return -1;
  }
  __kvx_getnanotime(&tp);
  tval->tv_sec = tp.tv_sec;
  tval->tv_usec = (tp.tv_nsec / 1000);
  return 0;
}

int gettimeofday(struct timeval *tval, void *tz) __attribute__ ((weak, alias ("_gettimeofday")));
