/* This file has been modified or added by Kalray, SA. 2010 */

#include <unistd.h>
#include "errno.h"



int sc_write(int fd, const void *buf, size_t len);

int __gloss_rm_write(int fd, const void *buf, size_t len)
{
  int ret;

  ret = sc_write(fd,buf,len);

  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

/***************************************************************
 *	write
 ***************************************************************/
int _write(int fd, const void *buf, size_t len)
{
   return __gloss_rm_write(fd, buf, len);
}

int write(int fd, const void *buf, size_t len) __attribute__ ((weak, alias ("_write")));
