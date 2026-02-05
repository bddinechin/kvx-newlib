/* Kalray SA. 2022, all rights reserved.
 * From gprof documentation, section 9.1. Implementation of Profiling:
 *   "Profiling works by changing how every function in your program
 *   is compiled so that when it is called, it will stash away some
 *   information about where it was called from.
 *   From this, the profiler can figure out what function called it,
 *   and can count how many times it was called.
 *   This change is made by the compiler when your program is
 *   compiled with the '-pg' option, which causes every function
 *   to call mcount [...] as one of its first operations.
 *
 *   The mcount routine, included in the profiling library,
 *   is responsible for recording in an in-memory call graph table
 *   both its parent routine (the child) and its parent's parent.
 *   This is typically done by examining the stack frame to find
 *   both the address of the child, and the return address in
 *   the original parent. Since this is a very machine-dependent
 *   operation, mcount itself is typically a short assembly-language
 *   stub routine that extracts the required information,
 *   and then calls __mcount_internal [...]"
 *
 * Default configurations not requiring the frame pointer,
 * note this stub is only effective when enforcing its use.
 * (eg. w/ GCC '-fno-omit-frame-pointer')
 */
.text
.global __mcount
.type __mcount, @function
.extern __mcount_internal
.type __mcount_internal, @function

__mcount:
  .cfi_startproc
  # Make room for saved arguments
  addd $r12 = $r12, -0x68
  ;;
  # Save arguments
  so 0x40[$r12] = $r0r1r2r3
  .cfi_def_cfa_offset 0x68
  ;;
  so 0x20[$r12] = $r4r5r6r7
  ;;
  so 0x00[$r12] = $r8r9r10r11
  ;;
  # Get caller's FP
  ld $r0 = 0x00[$r14]
  # Get return address
  get $r1 = $ra
  ;;
  # Save return address on the stack
  sd 0x60[$r12] = $r1
  # Call the real mcount
  call __mcount_internal
  ;;
  # Restore return address
  ld $r0 = 0x60[$r12]
  ;;
  set $ra = $r0
  # Restore arguments
  lo $r8r9r10r11 = 0x00[$r12]
  ;;
  lo $r4r5r6r7 = 0x20[$r12]
  ;;
  lo $r0r1r2r3 = 0x40[$r12]
  # Restore stack pointer
  addd $r12 = $r12, 0x68
  ret
  .cfi_endproc
  ;;
.size __mcount, .-__mcount
