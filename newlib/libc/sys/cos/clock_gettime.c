/*
 * Copyright 2021
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

#define _GNU_SOURCE
#include <errno.h>
#include <time.h>
#include <sys/time.h>

int
clock_gettime(clockid_t clock_id, struct timespec *tp)
{
  struct timeval tv;
  int retval = -1;

  if (!tp) {
    errno = EINVAL;
  } else {
    switch (clock_id)
    {
      case CLOCK_REALTIME:
      case CLOCK_MONOTONIC:
      case CLOCK_THREAD_CPUTIME_ID:
      case CLOCK_MONOTONIC_RAW:
      case CLOCK_MONOTONIC_COARSE:
      case CLOCK_BOOTTIME:
      case CLOCK_REALTIME_ALARM:
      case CLOCK_BOOTTIME_ALARM:
        retval = gettimeofday (&tv, NULL);
        if (retval == 0) {
          tp->tv_sec = tv.tv_sec;
          tp->tv_nsec = tv.tv_usec * 1000;
        }
        break;
      default:
	errno = EINVAL;
	break;
    }
  }

  return retval;
}
