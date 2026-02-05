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

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <hal/cos_defs.h>

static const char
__libc_cos_private_host_name_default[] =
#if defined(__kvxarch_kv3_1)
"ClusterOS-CV1";
#elif defined(__kvxarch_kv3_2)
"ClusterOS-CV2";
#elif defined(__kvxarch_kv4_1)
"ClusterOS-DV1";
#elif defined(__riscv)
"ClusterOS-RISCV";
#else
#error "Unsupported clusterOS chip version"
#endif

#if defined (__soc_mppa3_80_1) || (__soc_mppa3_80_2)
__COS_USER_SMEM_DATA_SECTION__
#endif //soc
static char *__libc_cos_private_host_name = NULL;

int
sethostname(const char *name, size_t len)
{
	int ret = -1;
	char *str = __libc_cos_private_host_name;

	if (str) {
		free(str);
	}
	str = malloc(len);
	if (!str) {
		errno = ENOMEM;
		goto early_exit;
	}
	strncpy(str, name, len);
	ret = 0;
early_exit:
	__libc_cos_private_host_name = str;
	return ret;
}

int
gethostname(char *name, size_t len)
{
	const char *str = __libc_cos_private_host_name ?
		__libc_cos_private_host_name : (const char *) __libc_cos_private_host_name_default;

	strncpy(name, str, len);
	return 0;
}
