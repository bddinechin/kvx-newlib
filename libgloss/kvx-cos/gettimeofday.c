/* This file has been modified or added by Kalray, SA. 2010 */

#include "errno.h"
#include <sys/time.h>
#include <time.h>

#include <hal/cos_power.h>
#include <hal/cos_cpu.h>

/***************************************************************
 *	gettimeofday
 ***************************************************************/

#define NANOSEC_PER_SECOND (1000000000uLL)

int __gloss_getnanotime(struct timespec *tval)
{
	unsigned long long cycles = mppa_pwr_ctrl_local->dsu_timestamp.reg;
	unsigned long long freq = _COS_TIMESTAMP_FREQ;
	tval->tv_sec = cycles / freq;
	tval->tv_nsec = (long)(((double)cycles - (double)freq*(double)tval->tv_sec) * ((double)NANOSEC_PER_SECOND/(double)freq));

	return 0;
}

int _gettimeofday(struct timeval *tval, void *tz)
{
	struct timespec tp;
	if(!tval){
			errno = EINVAL;
			return -1;
	}
	__gloss_getnanotime(&tp);
	tval->tv_sec = tp.tv_sec;
	tval->tv_usec = (tp.tv_nsec / 1000);
	return 0;
}

int gettimeofday(struct timeval *tval, void *tz) __attribute__ ((weak, alias ("_gettimeofday")));
