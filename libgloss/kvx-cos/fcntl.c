/* This file has been modified or added by Kalray, SA. 2010 */

#include <hal/cos_scall_no.h>

#include <fcntl.h>
#include <stdarg.h>
#include "errno.h"

/***************************************************************
 *	fcntl
 ***************************************************************/
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
