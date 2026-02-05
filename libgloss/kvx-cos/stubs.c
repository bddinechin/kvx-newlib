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

/*
 * This file implements stubs to unimplemented function
 * to let libc link correctly. Errors are returned for
 * each function. If some of the following functions
 * get implemented in the future, please move them
 * to another file in libgloss/kvx-cos/function_name.c
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <errno.h>
#include <pwd.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

int pipe(int pipefd[2])
{
	errno = -ENOSYS;
	return -1;
}

pid_t waitpid(pid_t pid, int *wstatus, int options)
{
	errno = -ENOSYS;
	return -1;
}

int issetugid(void)
{
	errno = -ENOSYS;
	return -1;
}

pid_t vfork(void)
{
	errno = -ENOSYS;
	return -1;
}

int sigaction(int signum, const struct sigaction *act,
	struct sigaction *oldact)
{
	errno = -ENOSYS;
	return -1;
}

int execvpe(const char *file, char *const argv[],
	char *const envp[])
{
	errno = -ENOSYS;
	return -1;
}

int seteuid(uid_t euid)
{
	errno = -ENOSYS;
	return -1;
}

int setegid(gid_t egid)
{
	errno = -ENOSYS;
	return -1;
}

int setpgid(pid_t pid, pid_t pgid)
{
	errno = -ENOSYS;
	return -1;
}

int sched_setparam(pid_t pid, const struct sched_param *param)
{
	errno = -ENOSYS;
	return -1;
}

int mknod(const char *__path, mode_t __mode, dev_t __dev)
{
	errno = -ENOSYS;
	return -1;
}

char *getlogin(void)
{
	errno = -ENOSYS;
	return NULL;
}

struct passwd *getpwnam(const char *name)
{
	errno = -ENOSYS;
	return NULL;
}

struct passwd *getpwuid(uid_t uid)
{
	errno = -ENOSYS;
	return NULL;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
	errno = -ENOSYS;
	return -1;
}

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
	errno = -ENOSYS;
	return -1;
}

long _pathconf(const char *path, int name) {
        errno = -EINVAL;
        return -1;
}

long pathconf(const char *path, int name)
        __attribute__ ((weak, alias ("_pathconf")));

char* _getcwd(char *buf, size_t size) {
        errno = -EINVAL;
        return NULL;
}

char* getcwd(char *buf, size_t size) __attribute__ ((weak, alias ("_getcwd")));
