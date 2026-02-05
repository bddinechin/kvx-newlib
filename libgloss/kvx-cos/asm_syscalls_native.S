#include "kvx/builtins_kvx_io_trap_port.h"

#define syscall_kvm_trap(name) \
            push %ebp                   ;\
            movl %esp,  %ebp            ;\
            subl $12,   %esp ;\
            movl $0,             (%esp) ;\
            movl $__NR_##name,  4(%esp) ;\
            movl %ebp,          8(%esp) ;\
            movl %esp,            %eax  ;\
            movw $_K1_SYSCALLS_BASEPORT, %dx ;\
            outl %eax,   %dx            ;\
            movl (%esp), %eax           ;\
            addl $12,    %esp           ;\
            popl %ebp

	.section .text, "ax", @progbits
	.align 4

#define SYSCALL_ENTRY(name) \
	.func sc_##name		; \
	.global sc_##name   	; \
sc_##name:


SYSCALL_ENTRY(access)
    syscall_kvm_trap(access)
	ret
.endfunc

SYSCALL_ENTRY(chdir)
    syscall_kvm_trap(chdir)
	ret
.endfunc

SYSCALL_ENTRY(chmod)
    syscall_kvm_trap(chmod)
	ret
.endfunc

SYSCALL_ENTRY(close)
    syscall_kvm_trap(close)
	ret
.endfunc

SYSCALL_ENTRY(dup2)
    syscall_kvm_trap(dup2)
	ret
.endfunc

SYSCALL_ENTRY(dup)
    syscall_kvm_trap(dup)
	ret
.endfunc

SYSCALL_ENTRY(exit)
    syscall_kvm_trap(exit)
	ret
.endfunc

SYSCALL_ENTRY(fcntl)
    syscall_kvm_trap(fcntl)
	ret
.endfunc

SYSCALL_ENTRY(fstat)
    syscall_kvm_trap(fstat)
	ret
.endfunc

SYSCALL_ENTRY(gettimeofday)
    syscall_kvm_trap(gettimeofday)
	ret
.endfunc

SYSCALL_ENTRY(isatty)
    syscall_kvm_trap(isatty)
	ret
.endfunc

SYSCALL_ENTRY(link)
    syscall_kvm_trap(link)
	ret
.endfunc

SYSCALL_ENTRY(lseek)
    syscall_kvm_trap(lseek)
	ret
.endfunc

SYSCALL_ENTRY(mkdir)
    syscall_kvm_trap(mkdir)
	ret
.endfunc

SYSCALL_ENTRY(mkfifo)
    syscall_kvm_trap(mkfifo)
	ret
.endfunc

SYSCALL_ENTRY(open)
    syscall_kvm_trap(open)
	ret
.endfunc

SYSCALL_ENTRY(read)
    syscall_kvm_trap(read)
	ret
.endfunc

SYSCALL_ENTRY(rmdir)
    syscall_kvm_trap(rmdir)
	ret
.endfunc

SYSCALL_ENTRY(stat)
    syscall_kvm_trap(stat)
	ret
.endfunc

SYSCALL_ENTRY(unlink)
    syscall_kvm_trap(unlink)
	ret
.endfunc

SYSCALL_ENTRY(write)
    syscall_kvm_trap(write)
	ret
.endfunc
