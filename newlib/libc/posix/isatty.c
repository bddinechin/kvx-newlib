#ifndef _NO_ISATTY

/* isatty.c */

#include <unistd.h>
#include <reent.h>

int
isatty (int fd)
{
  return _isatty (fd);
}

#endif /* !_NO_ISATTY */
