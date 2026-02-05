/*
 * Copyright 2021
 * Kalray Inc. All rights reserved.
 *
 * This software is furnished under license and may be used and copied only
 * in accordance with the following terms and conditions.  Subject to these
 * conditions, you may download, copy, install, use, modify and distribute
 * modified or unmodified copies of this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce and
 *    retain this copyright notice and list of conditions as they appear in
 *    the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Kalray Inc.  The "Kalray Inc" name may not be
 *    used to endorse or promote products derived from this software
 *    without the prior written permission of Kalray Inc.
 *
 * 3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 *    NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL KALRAY BE LIABLE
 *    FOR ANY DAMAGES WHATSOEVER, AND IN PARTICULAR, KALRAY SHALL NOT BE
 *    LIABLE FOR DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *    OR OTHERWISE), EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <malloc.h>
#include <hal/cos_syscall.h>
#include <hal/cos_scall_no.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void *mmap(void *addr, size_t length,
	int prot, int flags, int fd, off_t offset)
{
	void *ret = MAP_FAILED;
	cos_syscall_type_t arg0, arg1, arg2, arg3, arg4, arg5;

	if (!length || prot > (PROT_NONE | PROT_READ | PROT_WRITE | PROT_EXEC) ||
		(flags != MAP_SHARED && flags != MAP_PRIVATE)) {
		errno = -EINVAL;
		goto exit_err;
	}
	arg0 = (cos_syscall_type_t)(uintptr_t)addr;
	arg1 = (cos_syscall_type_t)length;
	arg2 = (cos_syscall_type_t)prot;
	arg3 = (cos_syscall_type_t)flags;
	arg4 = (cos_syscall_type_t)fd;
	arg5 = (cos_syscall_type_t)offset;
	ret = (void*)(uintptr_t)__cos_syscall6(__NR_mppa_cos_mmap, arg0, arg1, arg2, arg3, arg4, arg5);
	if (ret == MAP_FAILED) {
		errno = -EINVAL;
	}
exit_err:
	return ret;
}

int munmap(void *addr, size_t length)
{
	int ret;
	cos_syscall_type_t arg0, arg1;

	if (!addr || !length) {
		errno = -EINVAL;
		ret = -1;
		goto exit_err;
	}
	arg0 = (cos_syscall_type_t)(uintptr_t)addr;
	arg1 = (cos_syscall_type_t)length;
	ret =__cos_syscall2(__NR_mppa_cos_munmap, arg0, arg1);
	if (ret) {
		errno = -EINVAL;
	}
exit_err:
	return ret;
}

int mlockall(int flags)
{
	int ret = 0;

	if (!(flags & (MCL_CURRENT | MCL_FUTURE | MCL_ONFAULT))) {
		errno = -EINVAL;
		ret = -1;
	}

	/*
	 * This function does nothing but testing errors
	 * since clusterOS maps and allocates statically
	 * the physical memory
	 */
	return ret;
}

int munlockall(void)
{
	/*
	 * This function does nothing since
	 * clusterOS maps and allocates statically
	 * the physical memory
	 */
	return 0;
}

int shm_open(const char *name, int oflag, mode_t mode)
{
	errno = -ENOSYS;
	return -1;
}

int shm_unlink(const char *name)
{
	errno = -ENOSYS;
	return -1;
}

int mlock(const void *addr, size_t len)
{
	int ret = 0;

	if (!addr || !len) {
		errno = -EINVAL;
		ret = -1;
	}

	/*
	 * This function does nothing since
	 * clusterOS maps and allocates statically
	 * the physical memory
	 */
	return ret;
}

int munlock(const void *addr, size_t len)
{
	int ret = 0;

	if (!addr || !len) {
		errno = -EINVAL;
		ret = -1;
	}

	/*
	 * This function does nothing since
	 * clusterOS maps and allocates statically
	 * the physical memory
	 */
	return ret;
}

int madvise(void *addr, size_t length, int advice)
{
	int ret = 0;

	if (!addr || !length || advice < MADV_NORMAL || advice > MADV_PROTECT) {
		errno = -EINVAL;
		ret = -1;
	}

	/*
	 * This function does nothing since
	 * clusterOS maps and allocates statically
	 * the physical memory
	 */
	return ret;
}

int posix_madvise(void *addr, size_t len, int advice)
{
	return madvise(addr, len, advice);
}

int mprotect(const void *addr, size_t len, int prot)
{
	int ret = -1;
	cos_syscall_type_t arg0, arg1, arg2;

	if (!addr || !len || prot > (PROT_NONE | PROT_READ | PROT_WRITE | PROT_EXEC)) {
		errno = -EINVAL;
		goto exit_err;
	}
	arg0 = (cos_syscall_type_t)(uintptr_t)addr;
	arg1 = (cos_syscall_type_t)len;
	arg2 = (cos_syscall_type_t)prot;
	if (__cos_syscall3(__NR_mppa_cos_mprotect, arg0, arg1, arg2) != 0) {
		errno = -EINVAL;
		goto exit_err;
	}
	ret = 0;
exit_err:
	return ret;
}

int msync(void *addr, size_t length, int flags)
{

	if (!addr || !length || flags < MS_SYNC || flags > MS_INVALIDATE) {
		errno = -EINVAL;
		return -1;
	}
	/* No fs so nothing to do */
	return 0;
}
