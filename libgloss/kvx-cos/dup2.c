/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	dup2
 ***************************************************************/
int sc_dup2(int fd1, int fd2);

int _dup2(int fd1, int fd2)
{
  int ret;

  ret = sc_dup2(fd1,fd2);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int dup2(int fd1, int fd2) __attribute__ ((weak, alias ("_dup2")));
