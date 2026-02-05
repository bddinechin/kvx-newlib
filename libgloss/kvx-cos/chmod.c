/* This file has been modified or added by Kalray, SA. 2010 */

#include <sys/types.h>
#include "errno.h"

/***************************************************************
 *	chmod
 ***************************************************************/
int sc_chmod(const char *file, mode_t mode);

int _chmod(const char *file, mode_t mode)
{
  int ret;

  ret = sc_chmod(file,mode);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int chmod(const char *file, mode_t mode) __attribute__ ((weak, alias ("_chmod")));
