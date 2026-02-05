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

/* Implementation of non-OS memory allocation */

#include <unistd.h>
#include <errno.h>

extern void *_sbrk (ptrdiff_t);

extern char _heap_start ;
extern char _heap_end;
static char * _heap_ptr = NULL; /* calulated using symbols from the linker script: _heap_start and _heap_end */
static char *_end_of_allocatable_mem = NULL;

static int abs(int i)
{
  return (i < 0) ? -i : i;
}

void *_sbrk(ptrdiff_t nbytes) {
  /*  Arguments are:
   *  - number of bytes of memory to allocate
   *  This call atomically checks for overflow and increments the heap
   *  pointer if all is ok.
   *  The memory layout has the heap area starting above the user code and
   *  static data and growing up, and the stack starting in high memory
   *  (below kernel stack and environment, etc.) and growing down.
   */

  void * old_heap_ptr;
  void * returnCode;
  unsigned long initial_heap = 0;

  if (_heap_ptr == NULL) {
    unsigned long pagesz = getpagesize();

    /*
     * Align heap start on a page boundary like further allocations
     * in mallocr.c. This avoids differences in cache behavior due to
     * small increases of sections size.
     */
    _heap_ptr = (char *) ((((unsigned long) &_heap_start) + (pagesz - 1)) &
			  ~(pagesz - 1));
    initial_heap = (unsigned long) _heap_ptr;
    _end_of_allocatable_mem = &_heap_end;
  }
  old_heap_ptr = _heap_ptr ;

  /* Check overflows */
  if (nbytes >= 0) {
    if (! (nbytes <= ((unsigned long) _end_of_allocatable_mem -
		      (unsigned long) _heap_ptr))) {
      goto bad;
    }
  } else {
    if (! (abs(nbytes) <= ((unsigned long) old_heap_ptr - initial_heap))) {
      goto bad;
    }
  }

  _heap_ptr += nbytes;
  returnCode = old_heap_ptr;

  return returnCode;

 bad:
  errno = ENOMEM;
  return ((void *) -1);
}

void *sbrk(ptrdiff_t nbytes) __attribute__ ((weak, alias ("_sbrk")));
