/*
 * Stub version of getpid.
 */

#include <errno.h>

#include <hal/cos_bsp.h>

int
_getpid ()
{
  const int pcid = mppa_cos_get_physical_cluster_id();

  return 0x1000 | (pcid << 4);
}

int getpid ()
    __attribute__ ((weak, alias ("_getpid")));
