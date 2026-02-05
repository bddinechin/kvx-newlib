#include "sys/types.h"
#include "errno.h"

/***************************************************************
 *	mkfifo
 ***************************************************************/
int sc_mkfifo(const char *__path, mode_t __mode);

int _mkfifo(const char *__path, mode_t __mode)
{
  int ret;

  ret = sc_mkfifo(__path,__mode);

  if(ret < 0) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int mkfifo(const char *__path, mode_t __mode) __attribute__ ((weak, alias ("_mkfifo")));
