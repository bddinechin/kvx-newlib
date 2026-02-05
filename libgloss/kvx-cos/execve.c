/*
 * Stub version of execve.
 */

#include <errno.h>

int
_execve (char  *name, char **argv, char **env)
{
  errno = ENOSYS;
  return -1;
}

int execve (const char *dir, char **argv, char **env)
    __attribute__ ((weak, alias ("_execve")));
