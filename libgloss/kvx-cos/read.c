/* This file has been modified or added by Kalray, SA. 2010 */

#include <unistd.h>
#include "errno.h"

/***************************************************************
 *	read
 ***************************************************************/
int sc_read(int fd, void *buf, size_t len);

int __gloss_read(int fd, void *buf, size_t len){
  int ret;

  ret = sc_read(fd,buf,len);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int _read(int fd, void *buf, size_t len)
{
  return __gloss_read(fd, buf, len);
}

int read(int fd, void *buf, size_t len) __attribute__ ((weak, alias ("_read")));
