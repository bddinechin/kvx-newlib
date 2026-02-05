/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"

/***************************************************************
 *	link
 ***************************************************************/
int sc_link(const char *old, const char * new);

int _link(const char *old, const char * new)
{
    int ret;

    ret = sc_link(old,new);
    if( ret < 0 ) {
      errno = ret * -1;
      ret = -1;
    }
    return ret;
}

int link(const char *old, const char * new) __attribute__ ((weak, alias ("_link")));
