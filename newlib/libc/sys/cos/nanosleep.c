#include <errno.h>
#include <time.h>
#include <hal/cos_power.h>
#include <hal/cos_cpu.h>
#include <sys/types.h>
#include <stdint.h>
#include <pthread.h>

#define NANOSEC_PER_SECOND (1000000000uLL)

int nanosleep(const struct timespec *req, struct timespec *rem){
	const unsigned long long start = mppa_pwr_ctrl_local->dsu_timestamp.reg;
	const float freq = (float)_COS_TIMESTAMP_FREQ;
	int ret = EINVAL;

	if (req) {
		const unsigned long long nsec = req->tv_nsec;
		const int sec = req->tv_sec;

		if (nsec > (NANOSEC_PER_SECOND-1) || sec < 0)
			goto early_exit;

		const unsigned long long ticks = (unsigned long long)(((float)sec + (float)nsec / (float)NANOSEC_PER_SECOND) * freq);

		while (ticks > (mppa_pwr_ctrl_local->dsu_timestamp.reg - start)) {
			/* Let other threads run and
			 * handle cancellation points.
			 */
			pthread_yield();
		}

		if (rem) {
			rem->tv_nsec = rem->tv_sec = 0;
		}
		ret = 0;
	}
early_exit:
	return ret;
}
