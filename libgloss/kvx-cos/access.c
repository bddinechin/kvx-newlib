/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	access
 ***************************************************************/
extern int sc_access (const char *path, int mode);

int _access (const char *path, int mode)
{
    int ret;

    ret = sc_access(path,mode);
    if( ret < 0 ) {
      errno = ret * -1;
      ret = -1;
    }
    return ret;
}

int access (const char *path, int mode) __attribute__ ((weak, alias ("_access")));
