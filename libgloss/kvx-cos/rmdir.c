/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	rmdir
 ***************************************************************/
int sc_rmdir(const char *dir);

int _rmdir(const char *dir)
{
  int ret;

  ret = sc_rmdir(dir);

  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int rmdir(const char *dir) __attribute__ ((weak, alias ("_rmdir")));
