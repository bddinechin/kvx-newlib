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

int sc_fcntl(int fd, int command, long arg);

int _fcntl(int fd, int command, long arg)
{
  int ret;
  int oflags = arg;

  if (command == F_SETFL || command == F_GETFL) {
    if(command == F_SETFL) {
      oflags = 0;
      if (arg & O_RDONLY)   oflags |= S_RDONLY;
      if (arg & O_WRONLY)   oflags |= S_WRONLY;
      if (arg & O_RDWR)     oflags |= S_RDWR;
      if (arg & O_APPEND)   oflags |= S_APPEND;
      if (arg & O_CREAT)    oflags |= S_CREAT;
      if (arg & O_TRUNC)    oflags |= S_TRUNC;
      if (arg & O_EXCL)     oflags |= S_EXCL;
      if (arg & O_SYNC)     oflags |= S_SYNC;
#ifdef O_NDELAY
      if (arg & O_NDELAY)   oflags |= S_NDELAY;
#endif
      if (arg & O_NONBLOCK) oflags |= S_NONBLOCK;
      if (arg & O_NOCTTY)   oflags |= S_NOCTTY;
    }

    ret = sc_fcntl(fd,command,oflags);

    if( ret < 0 ) {
      errno = ret * -1;
      ret = -1;
    } else if (command == F_GETFL) {
      arg = ret;
      ret = 0;

      if (arg & S_RDONLY)   ret |= O_RDONLY;
      if (arg & S_WRONLY)   ret |= O_WRONLY;
      if (arg & S_RDWR)     ret |= O_RDWR;
      if (arg & S_APPEND)   ret |= O_APPEND;
      if (arg & S_CREAT)    ret |= O_CREAT;
      if (arg & S_TRUNC)    ret |= O_TRUNC;
      if (arg & S_EXCL)     ret |= O_EXCL;
      if (arg & S_SYNC)     ret |= O_SYNC;
#ifdef O_NDELAY
      if (arg & S_NDELAY)   ret |= O_NDELAY;
#endif
      if (arg & S_NONBLOCK) ret |= O_NONBLOCK;
      if (arg & S_NOCTTY)   ret |= O_NOCTTY;
    }
  }
  else if( command == F_SETLK ||
	   command == F_SETLKW ||
	   command == F_GETLK ) {

    /* struct flock {
	 short	l_type;	        F_RDLCK, F_WRLCK, or F_UNLCK
	 short	l_whence;	flag to choose starting offset
	 long	l_start;	relative offset, in bytes
	 long	l_len;		length, in bytes; 0 means lock to EOF
	 short	l_pid;		returned with F_GETLK
	 short	l_xxx;		reserved for future use
       };
    */

    struct flock *fl = (struct flock*)(void*)arg;
    uint64_t res[6];
    res[0] = (uint64_t) fl->l_type;
    res[1] = (uint64_t) fl->l_whence;
    res[2] = (uint64_t) fl->l_start;
    res[3] = (uint64_t) fl->l_len;
    res[4] = (uint64_t) fl->l_pid;
    res[5] = (uint64_t) fl->l_xxx;

    ret = sc_fcntl(fd,command,(long)(uintptr_t)res);

    if( ret < 0 ) {
      errno = ret * -1;
      ret = -1;
    }
    else {
      fl->l_type = (short)res[0];
      fl->l_whence = (short)res[1];
      fl->l_start = (long)res[2];
      fl->l_len = (long)res[3];
      fl->l_pid = (short)res[4];
      fl->l_xxx = (short)res[5];
    }
  }
  else {
    /* Command not supported yet */
    errno = ENOSYS;
    ret = -1;
  }

  return ret;
}

int fcntl(int fd, int command, ...)  __attribute__ ((weak, alias ("_fcntl")));
