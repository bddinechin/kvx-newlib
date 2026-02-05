/* This file has been modified or added by Kalray, SA. 2010 */

#include <sys/types.h>
#include "errno.h"

/***************************************************************
 *	mkdir
 ***************************************************************/
int sc_mkdir(const char *path, mode_t mode);

int _mkdir(const char *path, mode_t mode)
{
  int ret;

  ret = sc_mkdir(path,mode);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int mkdir(const char *path, mode_t mode) __attribute__ ((weak, alias ("_mkdir")));
