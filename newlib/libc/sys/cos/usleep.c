#include <errno.h>
#include <time.h>
#include <unistd.h>

#define TOD_MICROSECONDS_PER_SECOND     1000000


int usleep(useconds_t useconds){
  struct timespec t;

  t.tv_sec = useconds / TOD_MICROSECONDS_PER_SECOND;
  t.tv_nsec = (useconds % TOD_MICROSECONDS_PER_SECOND) * 1000;

  return nanosleep(&t, NULL);

}
