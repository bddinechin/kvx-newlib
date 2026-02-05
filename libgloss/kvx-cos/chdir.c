/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	chdir
 ***************************************************************/
int sc_chdir ( const char *dir);

int _chdir ( const char *dir)
{
  int ret;

  ret = sc_chdir(dir);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int chdir ( const char *dir) __attribute__ ((weak, alias ("_chdir")));
