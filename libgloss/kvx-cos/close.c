/* This file has been modified or added by Kalray, SA. 2010 */

#include <unistd.h>
#include "errno.h"

/***************************************************************
 *	close
 ***************************************************************/
int sc_close(int fd);

int __gloss_close(int fd){
  int ret;
  ret = sc_close(fd);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int _close(int fd)
{
	return __gloss_close(fd);
}

int close(int fd) __attribute__ ((weak, alias ("_close")));
