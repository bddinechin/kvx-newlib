#include <unistd.h>

/* Used by sbrk.c */
int
getpagesize ()
{
  return 4096;
}
