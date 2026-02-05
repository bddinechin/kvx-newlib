/*
 * Stub version of wait.
 */

#include <errno.h>

int
_wait (int status)
{
  errno = ENOSYS;
  return -1;
}

int wait (int status)
    __attribute__ ((weak, alias ("_wait")));
