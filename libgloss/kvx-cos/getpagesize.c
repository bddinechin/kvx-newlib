/* This file has been modified or added by Kalray, SA. 2010 */

#include <unistd.h>

/* Used by mallocr.c and sbrk.c */
int getpagesize()
{
  return 4096;
}
