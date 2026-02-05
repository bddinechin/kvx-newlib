#include <hal/cos_scall_no.h>

#if defined(__native__)
#include "asm_syscalls_native.S"
#else

	.section .text, "ax", @progbits
	.align 4

.cfi_sections .debug_frame

#define SYSCALL_ENTRY(name) \
	.globl sc_##name   	; \
sc_##name: ; \
	.cfi_startproc

#define SYSCALL_END_ENTRY(name) \
	.cfi_endproc ; \
	.type sc_##name, @function ; \
	.size sc_##name, .-sc_##name

SYSCALL_ENTRY(access)
	scall __NR_access
	;;
	ret
	;;
SYSCALL_END_ENTRY(access)

SYSCALL_ENTRY(chdir)
	scall __NR_chdir
	;;
	ret
	;;
SYSCALL_END_ENTRY(chdir)

SYSCALL_ENTRY(chmod)
	scall __NR_chmod
	;;
	ret
	;;
SYSCALL_END_ENTRY(chmod)

SYSCALL_ENTRY(close)
	scall __NR_close
	;;
	ret
	;;
SYSCALL_END_ENTRY(close)

SYSCALL_ENTRY(dup2)
	scall __NR_dup2
	;;
	ret
	;;
SYSCALL_END_ENTRY(dup2)

SYSCALL_ENTRY(dup)
	scall __NR_dup
	;;
	ret
	;;
SYSCALL_END_ENTRY(dup)

SYSCALL_ENTRY(exit)
	scall __NR_exit
	;;
	ret
	;;
SYSCALL_END_ENTRY(exit)

SYSCALL_ENTRY(fcntl)
	scall __NR_fcntl
	;;
	ret
	;;
SYSCALL_END_ENTRY(fcntl)

SYSCALL_ENTRY(fstat)
	scall __NR_fstat
	;;
	ret
	;;
SYSCALL_END_ENTRY(fstat)

SYSCALL_ENTRY(gettimeofday)
	scall __NR_gettimeofday
	;;
	ret
	;;
SYSCALL_END_ENTRY(gettimeofday)

SYSCALL_ENTRY(isatty)
	scall __NR_isatty
	;;
	ret
	;;
SYSCALL_END_ENTRY(isatty)

SYSCALL_ENTRY(link)
	scall __NR_link
	;;
	ret
	;;
SYSCALL_END_ENTRY(link)

SYSCALL_ENTRY(lseek)
	scall __NR_lseek
	;;
	ret
	;;
SYSCALL_END_ENTRY(lseek)

SYSCALL_ENTRY(mkdir)
	scall __NR_mkdir
	;;
	ret
	;;
SYSCALL_END_ENTRY(mkdir)

SYSCALL_ENTRY(getdents)
	scall __NR_getdents
	;;
	ret
	;;
SYSCALL_END_ENTRY(getdents)

SYSCALL_ENTRY(getuid)
	scall __NR_getuid
	;;
	ret
	;;
SYSCALL_END_ENTRY(getuid)

SYSCALL_ENTRY(geteuid)
	scall __NR_geteuid
	;;
	ret
	;;
SYSCALL_END_ENTRY(geteuid)

SYSCALL_ENTRY(getgid)
	scall __NR_getgid
	;;
	ret
	;;
SYSCALL_END_ENTRY(getgid)

SYSCALL_ENTRY(getegid)
	scall __NR_getegid
	;;
	ret
	;;
SYSCALL_END_ENTRY(getegid)

SYSCALL_ENTRY(mkfifo)
	scall __NR_iss_mkfifo
	;;
	ret
	;;
SYSCALL_END_ENTRY(mkfifo)

SYSCALL_ENTRY(open)
	scall __NR_open
	;;
	ret
	;;
SYSCALL_END_ENTRY(open)

SYSCALL_ENTRY(read)
	scall __NR_read
	;;
	ret
	;;
SYSCALL_END_ENTRY(read)

SYSCALL_ENTRY(rmdir)
	scall __NR_rmdir
	;;
	ret
	;;
SYSCALL_END_ENTRY(rmdir)

SYSCALL_ENTRY(stat)
	scall __NR_stat
	;;
	ret
	;;
SYSCALL_END_ENTRY(stat)

SYSCALL_ENTRY(lstat)
	scall __NR_lstat
	;;
	ret
	;;
SYSCALL_END_ENTRY(lstat)

SYSCALL_ENTRY(unlink)
	scall __NR_unlink
	;;
	ret
	;;
SYSCALL_END_ENTRY(unlink)

SYSCALL_ENTRY(write)
	scall __NR_write
	;;
	ret
	;;
SYSCALL_END_ENTRY(write)

#endif
