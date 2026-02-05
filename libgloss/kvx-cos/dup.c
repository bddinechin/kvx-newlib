/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	dup
 ***************************************************************/
int sc_dup(int fd);

int _dup(int fd)
{
  int ret;

  ret = sc_dup(fd);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int dup(int fd) __attribute__ ((weak, alias ("_dup")));
