/*
 * Copyright 2019
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

#include <mppa_bare_runtime/kvx/scall_no.h>

	.section .text, "ax", @progbits
	.align 4

#define SYSCALL_ENTRY(name) \
	.proc sc_##name		; \
	.global sc_##name   	; \
sc_##name:


SYSCALL_ENTRY(access)
	scall __NR_access
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(chdir)
	scall __NR_chdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(chmod)
	scall __NR_chmod
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(close)
	scall __NR_close
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(dup2)
	scall __NR_dup2
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(dup)
	scall __NR_dup
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(exit)
	scall __NR_exit
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(fcntl)
	scall __NR_fcntl
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(fstat)
	scall __NR_fstat
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(gettimeofday)
	scall __NR_gettimeofday
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(isatty)
	scall __NR_isatty
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(link)
	scall __NR_link
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(lseek)
	scall __NR_lseek
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(mkdir)
	scall __NR_mkdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(mkfifo)
	scall __NR_iss_mkfifo
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(open)
	scall __NR_open
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(read)
	scall __NR_read
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(rmdir)
	scall __NR_rmdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(stat)
	scall __NR_stat
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(unlink)
	scall __NR_unlink
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(write)
	scall __NR_write
	;;
	ret
	;;
	.endp
