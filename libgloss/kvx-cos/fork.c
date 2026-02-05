/*
 * Stub version of fork.
 */

#include <errno.h>

int
_fork ()
{
  errno = ENOSYS;
  return -1;
}

int fork ()
    __attribute__ ((weak, alias ("_fork")));
