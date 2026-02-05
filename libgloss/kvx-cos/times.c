/*
 * Stub version of times.
 */

#include <sys/types.h>
#include <sys/times.h>
#include <errno.h>

#include <hal/cos_power.h>
#include <hal/cos_cpu.h>

int __gloss_times(struct tms *buf)
{
  /*  errno = ENOSYS; */
  unsigned long long dsu_cycles = mppa_pwr_ctrl_local->dsu_timestamp.reg;
  int cycle_ratio = _COS_CPU_FREQ / __COS_CLOCKS_PER_SEC__;
  clock_t cycles = dsu_cycles / cycle_ratio;
  /* User time */
  if (buf != NULL) {
    buf->tms_utime = cycles;
    /* System time */
    buf->tms_stime = 0;
    /* children's user time */
    buf->tms_cutime = 0;
    /* children's system time */
    buf->tms_cstime = 0;
  }
  return cycles;
}

clock_t
_times (struct tms *buf)
{
  return __gloss_times(buf);
}

clock_t times (struct tms *buf)
    __attribute__ ((weak, alias ("_times")));
