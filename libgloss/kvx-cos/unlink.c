/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	unlink
 ***************************************************************/
int sc_unlink(const char *filename);

int _unlink(const char *filename)
{
    int ret;

    ret = sc_unlink(filename);

    if( ret < 0 ) {
      errno = ret * -1;
      ret = -1;
    }
    return ret;
}

int unlink(const char *filename) __attribute__ ((weak, alias ("_unlink")));
