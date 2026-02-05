/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	isatty
 ***************************************************************/
int sc_isatty(int fd);

static inline int __gloss_isatty(int fd)
{
  int ret;

  ret = sc_isatty(fd);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int _isatty(int fd)
{
  return __gloss_isatty(fd);
}

int isatty(int fd) __attribute__ ((weak, alias ("_isatty")));
