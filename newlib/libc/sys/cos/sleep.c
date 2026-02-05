#include <errno.h>
#include <time.h>
#include <unistd.h>

unsigned int _sleep(unsigned int seconds)
{
   struct timespec t;

   t.tv_nsec = 0;
   t.tv_sec = seconds;

   return nanosleep(&t, NULL);
}

unsigned int sleep(unsigned int seconds)
    __attribute__ ((weak, alias ("_sleep")));
