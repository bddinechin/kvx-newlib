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

#include "asm.h"

/*
 * Saved layout is as follows:
 * 0x00: $r18 $r19
 * 0x10: $r20 $r21 $r22 $r23
 * 0x30: $r24 $r25 $r26 $r27
 * 0x50: $r28 $r29 $r30 $r31
 * 0x70: $r13 $r14 $ls  $le
 * 0x90: $lc  $cs  $ra  $r12
 *
 * Warning: $ra offset (0xa0) must be set in gdb port
 * in gdb/gdb/kvx-common-tdep.c
 */

    ## int setjmp(jmp_buf)
    ##
    ## Basically setjmp saves the set of callee saved registers in jmp_buf
    ## Also saving cs, the loop registers, ra and sp
_ENTRY(setjmp)
    .file 1 "setjmp.S"
    .loc 1 54 0
    ## All preserved registers
    sq 0x00[$r0] = $r18r19
    copyd   $r32 = $r13
    get     $r34 = $ls
;;
    so 0x10[$r0] = $r20r21r22r23
    copyd   $r33 = $r14
    get     $r35 = $le
;;
    so 0x30[$r0] = $r24r25r26r27
    get     $r36 = $lc
;;
    so 0x50[$r0] = $r28r29r30r31
    get     $r37 = $cs
;;
    so 0x70[$r0] = $r32r33r34r35
    get     $r38 = $ra
    copyd   $r39 = $r12
;;
    so 0x90[$r0] = $r36r37r38r39
    ## Must return zero if called to set up jmp_buf
    make     $r0 = 0
    ret
;;
    .endp setjmp



    ## void longjmp(jmp_buf env, int val)
    ##
    ## Basically restore the saved register set found in env to processor
    ## Restoring also sp. Note that it jumps back to original location
_ENTRY(longjmp)
        .loc 1 88 0
    lo $r32r33r34r35 = 0x70[$r0]
;;
    lo $r36r37r38r39 = 0x90[$r0]
    copyd   $r13 = $r32
    copyd   $r14 = $r33
    set     $ls = $r34
;;
    lq $r18r19 = 0x00[$r0]
    set     $le = $r35
;;
    lo $r20r21r22r23 = 0x10[$r0]
    set     $lc = $r36
;;
    lo $r24r25r26r27 = 0x30[$r0]
    set     $cs = $r37
;;
    lo $r28r29r30r31 = 0x50[$r0]
    set     $ra = $r38
    copyd   $r12 = $r39
;;
    ## Finally if called with val==0, return one else val
    maxud   $r0 = $r1, 1
    ret
;;
    .endp longjmp
