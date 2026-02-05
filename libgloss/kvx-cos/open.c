/* This file has been modified or added by Kalray, SA. 2010 */

#include <hal/cos_scall_no.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "errno.h"



/***************************************************************
 *	open
 ***************************************************************/
int sc_open(const char *name,int flags, int mode);

int __gloss_open(const char *name,int flags, int mode){
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

int _open(const char *name,int flags, int mode)
{
  return __gloss_open(name, flags, mode);
}

int open(const char *name,int flags, ...) __attribute__ ((weak, alias ("_open")));
