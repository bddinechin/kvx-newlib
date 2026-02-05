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

#include <mppa_bare_runtime/kvx/scall_no.h>

#include <fcntl.h>
#include <stdarg.h>
#include "errno.h"

int sc_open(const char *name,int flags, int mode);

int _open(const char *name,int flags, int mode)
{
  int ret = 0;
  int oflags = 0;

  if (flags & O_RDONLY)   oflags |= S_RDONLY;
  if (flags & O_WRONLY)   oflags |= S_WRONLY;
  if (flags & O_RDWR)     oflags |= S_RDWR;
  if (flags & O_APPEND)   oflags |= S_APPEND;
  if (flags & O_CREAT)    oflags |= S_CREAT;
  if (flags & O_TRUNC)    oflags |= S_TRUNC;
  if (flags & O_EXCL)     oflags |= S_EXCL;
  if (flags & O_SYNC)     oflags |= S_SYNC;
#ifdef O_NDELAY
  if (flags & O_NDELAY)   oflags |= S_NDELAY;
#endif
  if (flags & O_NONBLOCK) oflags |= S_NONBLOCK;
  if (flags & O_NOCTTY)   oflags |= S_NOCTTY;

  ret = sc_open(name,oflags,mode);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int open(const char *name,int flags, ...) __attribute__ ((weak, alias ("_open")));
