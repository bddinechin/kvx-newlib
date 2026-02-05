/* This file has been modified or added by Kalray, SA. 2010 */

#include <sys/types.h>
#include <unistd.h>
#include "errno.h"

/***************************************************************
 *	lseek
 ***************************************************************/
off_t sc_lseek(int fd, off_t offset, int whence);

off_t _lseek(int fd, off_t offset, int whence)
{

  int ret;

  ret = sc_lseek(fd,offset,whence);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

off_t lseek(int fd, off_t offset, int whence) __attribute__ ((weak, alias ("_lseek")));
