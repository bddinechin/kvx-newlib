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

/* Size above which we take the streaming load path */
#define STREAMING_THRESHOLD 256

/* Parameters:
   $r0: dst pointer
   $r1: src pointer
   $r2: length in bytes
*/

.cfi_sections .debug_frame
.text
.align 16
#ifdef MEMMOVE
.global memmove
.proc memmove
memmove:
   .cfi_startproc
/* For memmove, we first have to check for a potential overlap. If true,
   fall back to a backwards copy, otherwise take the memcpy path. */

   cb.deqz $r2? .Lreturn
   # $r5 = src + length
   addd $r5 = $r1, $r2
   # Save return value (dst)
   copyd $r6 = $r0
   addd $r12 = $r12, -64
   stcond o, .dnez, $r2, -32[$r12], $r28r29r30r31
   ;;
   .cfi_def_cfa_offset 64
   so 0[$r12] = $r24r25r26r27
   # $r4 = src < dst
   compd.ltu $r4 = $r1, $r0
   # $r7 = dst < $r5
   compd.ltu $r7 = $r0, $r5
   ;;
   andw $r4 = $r4, $r7
   ;;
   ## Bundle stalled for one cycle waiting for $r4
   # Go to backwards copy if an overlap is detected
   cb.wnez $r4? .Lbackwards_copy
   compd.geu $r3 = $r2, STREAMING_THRESHOLD
   ;;

/* No overlap, fall through memcpy path. */

#else
.global memcpy
.proc memcpy
memcpy:
   .cfi_startproc
   cb.deqz $r2? .Lreturn
   compd.geu $r3 = $r2, STREAMING_THRESHOLD
   stcond o, .dnez, $r2, -32[$r12], $r28r29r30r31
   copyd $r6 = $r0
   addd $r12 = $r12, -64
   ;;
   .cfi_def_cfa_offset 64
   so 0[$r12] = $r24r25r26r27
#endif
   cb.deqz $r3? .Lremaining_256
   ;;
   lq.u $r32r33 = 0[$r1]
   addd $r2 = $r2, -256
   andd $r7 = $r2, 255
   ;;
   lq.u $r34r35 = 16[$r1]
   srld $r7 = $r7, 5
   ;;
   lq.u $r36r37 = 32[$r1]
   ;;
   lq.u $r38r39 = 48[$r1]
   ;;
   lq.u $r40r41 = 64[$r1]
   compd.gt $r3 = $r7, 1
   compd.gt $r4 = $r7, 2
   compd.gt $r5 = $r7, 3
   ;;
   lq.u $r42r43 = 80[$r1]
   compd.gt $r8 = $r7, 4
   compd.gt $r9 = $r7, 5
   compd.gt $r10 = $r7, 6
   ;;
   lq.u $r44r45 = 96[$r1]
   andd $r24 = $r2, 16
   andd $r25 = $r2, 8
   andd $r26 = $r2, 4
   ;;
   lq.u $r46r47 = 112[$r1]
   andd $r27 = $r2, 2
   andd $r28 = $r2, 1
   ;;
   lq.u $r48r49 = 128[$r1]
   ;;
   lq.u $r50r51 = 144[$r1]
   ;;
   lq.u $r52r53 = 160[$r1]
   ;;
   lq.u $r54r55 = 176[$r1]
   ;;
   lq.u $r56r57 = 192[$r1]
   ;;
   lq.u $r58r59 = 208[$r1]
   ;;
   lq.u $r60r61 = 224[$r1]
   ;;
   lq.u $r62r63 = 240[$r1]
   addd $r1 = $r1, 256
   srld $r7 = $r2, 8
   ;;
   cb.deqz $r7? .Lstreaming_remaining_256
   ;;
   loopdo $r7, .Lstreaming_kernel_256
   ;;
   sq 0[$r0] = $r32r33
   addd $r2 = $r2, -256
   ;;
   lq.u $r32r33 = 0[$r1]
   ;;
   sq 16[$r0] = $r34r35
   ;;
   lq.u $r34r35 = 16[$r1]
   ;;
   sq 32[$r0] = $r36r37
   ;;
   lq.u $r36r37 = 32[$r1]
   ;;
   sq 48[$r0] = $r38r39
   ;;
   lq.u $r38r39 = 48[$r1]
   ;;
   sq 64[$r0] = $r40r41
   ;;
   lq.u $r40r41 = 64[$r1]
   ;;
   sq 80[$r0] = $r42r43
   ;;
   lq.u $r42r43 = 80[$r1]
   ;;
   sq 96[$r0] = $r44r45
   ;;
   lq.u $r44r45 = 96[$r1]
   ;;
   sq 112[$r0] = $r46r47
   ;;
   lq.u $r46r47 = 112[$r1]
   ;;
   sq 128[$r0] = $r48r49
   ;;
   lq.u $r48r49 = 128[$r1]
   ;;
   sq 144[$r0] = $r50r51
   ;;
   lq.u $r50r51 = 144[$r1]
   ;;
   sq 160[$r0] = $r52r53
   ;;
   lq.u $r52r53 = 160[$r1]
   ;;
   sq 176[$r0] = $r54r55
   ;;
   lq.u $r54r55 = 176[$r1]
   ;;
   sq 192[$r0] = $r56r57
   ;;
   lq.u $r56r57 = 192[$r1]
   ;;
   sq 208[$r0] = $r58r59
   ;;
   lq.u $r58r59 = 208[$r1]
   ;;
   sq 224[$r0] = $r60r61
   ;;
   lq.u $r60r61 = 224[$r1]
   ;;
   sq 240[$r0] = $r62r63
   addd $r0 = $r0, 256
   ;;
   lq.u $r62r63 = 240[$r1]
   addd $r1 = $r1, 256
   ;;
.Lstreaming_kernel_256:
.Lstreaming_remaining_256:
   so 0[$r0] = $r32r33r34r35
   addd $r11 = $r1, 32
   srld $r7 = $r2, 5
   ;;
   ldcond o, .u, .dnez, $r7, $r32r33r34r35, 0[$r1]
   cmoved.dnez $r7? $r1 = $r11
   compd.gt $r7 = $r7, 0
   ;;
   so 32[$r0] = $r36r37r38r39
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r3, $r36r37r38r39, 0[$r1]
   cmoved.dnez $r3? $r1 = $r11
   ;;
   so 64[$r0] = $r40r41r42r43
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r4, $r40r41r42r43, 0[$r1]
   cmoved.dnez $r4? $r1 = $r11
   ;;
   so 96[$r0] = $r44r45r46r47
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r5, $r44r45r46r47, 0[$r1]
   cmoved.dnez $r5? $r1 = $r11
   ;;
   so 128[$r0] = $r48r49r50r51
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r8, $r48r49r50r51, 0[$r1]
   cmoved.dnez $r8? $r1 = $r11
   ;;
   so 160[$r0] = $r52r53r54r55
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r9, $r52r53r54r55, 0[$r1]
   cmoved.dnez $r9? $r1 = $r11
   ;;
   so 192[$r0] = $r56r57r58r59
   addd $r11 = $r1, 32
   ;;
   ldcond o, .u, .dnez, $r10, $r56r57r58r59, 0[$r1]
   cmoved.dnez $r10? $r1 = $r11
   ;;
   so 224[$r0] = $r60r61r62r63
   addd $r0 = $r0, 256
   addd $r11 = $r1, 16
   ;;
   ldcond q, .u, .dnez, $r24, $r60r61, 0[$r1]
   cmoved.dnez $r24? $r1 = $r11
   ;;
   addd $r11 = $r1, 8
   ldcond d, .u, .dnez, $r25, $r62, 0[$r1]
   ;;
   cmoved.dnez $r25? $r1 = $r11
   ;;
   addd $r11 = $r1, 4
   ldcond wz, .u, .dnez,  $r26, $r63, 0[$r1]
   ;;
   cmoved.dnez $r26? $r1 = $r11
   ;;
   addd $r11 = $r1, 2
   ldcond hz, .u, .dnez, $r27, $r29, 0[$r1]
   ;;
   cmoved.dnez $r27? $r1 = $r11
   addd $r11 = $r10, $r9
   addd $r31 = $r5, $r4
   ;;
   ldcond bz, .u, .dnez, $r28, $r30, 0[$r1]
   addd $r11 = $r11, $r8
   addd $r31 = $r31, $r3
   ;;
   stcond o, .dnez, $r7, 0[$r0], $r32r33r34r35
   addd $r11 = $r11, $r7
   ;;
   stcond o, .dnez, $r3, 32[$r0], $r36r37r38r39
   addd $r11 = $r11, $r31
   ;;
   stcond o, .dnez, $r4, 64[$r0], $r40r41r42r43
   muld $r4 = $r11, 32
   ;;
   stcond o, .dnez, $r5, 96[$r0], $r44r45r46r47
   ;;
   stcond o, .dnez, $r8, 128[$r0], $r48r49r50r51
   addd $r32 = $r0, $r4
   ;;
   stcond o, .dnez, $r9, 160[$r0], $r52r53r54r55
   addd $r11 = $r32, 16
   addd $r8 = $r32, 0
   ;;
   stcond o, .dnez, $r10, 192[$r0], $r56r57r58r59
   cmoved.dnez $r24? $r8 = $r11
   ;;
   stcond q, .dnez, $r24, 0[$r32], $r60r61
   addd $r5 = $r8, 8
   addd $r0 = $r8, 0
   ;;
   stcond d, .dnez $r25, 0[$r8], $r62
   cmoved.dnez $r25? $r0 = $r5
   ;;
   stcond w, .dnez, $r26, 0[$r0], $r63
   addd $r5 = $r0, 4
   ;;
   cmoved.dnez $r26? $r0 = $r5
   ;;
   stcond h, .dnez, $r27, 0[$r0], $r29
   addd $r5 = $r0, 2
   ;;
   cmoved.dnez $r27? $r0 = $r5
   lo $r24r25r26r27 = 0[$r12]
   ;;
   stcond b, .dnez, $r28, 0[$r0], $r30
   ;;
   lo $r28r29r30r31 = 32[$r12]
   addd $r12 = $r12, 64
   copyd $r0 = $r6
   ret
   ;;
.Lremaining_256:
   andd $r11 = $r2, 16
   srld $r7 = $r2, 5
   ;;
   cb.deqz $r7? .Lloop_remaining_32
   ;;
   loopdo $r7, .Lloop_kernel_256
   ;;
   lo $r32r33r34r35 = 0[$r1]
   addd $r1 = $r1, 32
   addd $r2 = $r2, -32
   ;;
   so 0[$r0] = $r32r33r34r35
   addd $r0 = $r0, 32
   ;;
.Lloop_kernel_256:
.Lloop_remaining_32:
   andd $r10 = $r2, 8
   andd $r9 = $r2, 4
   cb.deqz $r11? .Lloop_remaining_16
   ldcond q, "", .dnez, $r11, $r32r33, 0[$r1]
   ;;
   sq 0[$r0] = $r32r33
   addd $r1 = $r1, 16
   addd $r0 = $r0, 16
   ;;
.Lloop_remaining_16:
   andd $r8 = $r2, 2
   andd $r7 = $r2, 1
   cb.deqz $r10? .Lloop_remaining_8
   ldcond d, "", .dnez, $r10, $r32, 0[$r1]
   ;;
   sd 0[$r0] = $r32
   addd $r1 = $r1, 8
   addd $r0 = $r0, 8
   ;;
.Lloop_remaining_8:
   cb.deqz $r9? .Lloop_remaining_4
   ldcond wz, "", .dnez, $r9, $r32, 0[$r1]
   ;;
   sw 0[$r0] = $r32
   addd $r1 = $r1, 4
   addd $r0 = $r0, 4
   ;;
.Lloop_remaining_4:
   cb.deqz $r8? .Lloop_remaining_2
   ldcond hz, "", .dnez, $r8, $r32, 0[$r1]
   ;;
   sh 0[$r0] = $r32
   addd $r1 = $r1, 2
   addd $r0 = $r0, 2
   ;;
.Lloop_remaining_2:
   ldcond bz, "", .dnez, $r7, $r32, 0[$r1]
   ;;
   stcond b, .dnez, $r7, 0[$r0], $r32
   ;;
.Lreturn:
   addd $r12 = $r12, 64
   copyd $r0 = $r6
   ret
   ;;

#ifdef MEMMOVE
/* Do the same as memcpy, except backwards. */
.Lbackwards_copy:
   # Move to the end of the buffers
   addd $r0 = $r0, $r2
   addd $r1 = $r1, $r2
   # Jump if length < STREAMING_THRESHOLD
   cb.deqz $r3? .Lbackwards_remaining_256
   ;;
   lq.u $r32r33 = -16[$r1]
   addd $r2 = $r2, -256
   ;;
   lq.u $r34r35 = -32[$r1]
   ;;
   lq.u $r36r37 = -48[$r1]
   srld $r7 = $r2, 8
   ;;
   lq.u $r38r39 = -64[$r1]
   ;;
   lq.u $r40r41 = -80[$r1]
   ;;
   lq.u $r42r43 = -96[$r1]
   ;;
   lq.u $r44r45 = -112[$r1]
   ;;
   lq.u $r46r47 = -128[$r1]
   ;;
   lq.u $r48r49 = -144[$r1]
   ;;
   lq.u $r50r51 = -160[$r1]
   ;;
   lq.u $r52r53 = -176[$r1]
   ;;
   lq.u $r54r55 = -192[$r1]
   ;;
   lq.u $r56r57 = -208[$r1]
   ;;
   lq.u $r58r59 = -224[$r1]
   ;;
   lq.u $r60r61 = -240[$r1]
   ;;
   lq.u $r62r63 = -256[$r1]
   addd $r1 = $r1, -256
   ;;
   cb.deqz $r7? .Lbackwards_streaming_remaining_256
   ;;
   loopdo $r7, .Lbackwards_streaming_kernel_256
   ;;
   sq -16[$r0] = $r32r33
   addd $r2 = $r2, -256
   ;;
   lq.u $r32r33 = -16[$r1]
   ;;
   sq -32[$r0] = $r34r35
   ;;
   lq.u $r34r35 = -32[$r1]
   ;;
   sq -48[$r0] = $r36r37
   ;;
   lq.u $r36r37 = -48[$r1]
   ;;
   sq -64[$r0] = $r38r39
   ;;
   lq.u $r38r39 = -64[$r1]
   ;;
   sq -80[$r0] = $r40r41
   ;;
   lq.u $r40r41 = -80[$r1]
   ;;
   sq -96[$r0] = $r42r43
   ;;
   lq.u $r42r43 = -96[$r1]
   ;;
   sq -112[$r0] = $r44r45
   ;;
   lq.u $r44r45 = -112[$r1]
   ;;
   sq -128[$r0] = $r46r47
   ;;
   lq.u $r46r47 = -128[$r1]
   ;;
   sq -144[$r0] = $r48r49
   ;;
   lq.u $r48r49 = -144[$r1]
   ;;
   sq -160[$r0] = $r50r51
   ;;
   lq.u $r50r51 = -160[$r1]
   ;;
   sq -176[$r0] = $r52r53
   ;;
   lq.u $r52r53 = -176[$r1]
   ;;
   sq -192[$r0] = $r54r55
   ;;
   lq.u $r54r55 = -192[$r1]
   ;;
   sq -208[$r0] = $r56r57
   ;;
   lq.u $r56r57 = -208[$r1]
   ;;
   sq -224[$r0] = $r58r59
   ;;
   lq.u $r58r59 = -224[$r1]
   ;;
   sq -240[$r0] = $r60r61
   ;;
   lq.u $r60r61 = -240[$r1]
   ;;
   sq -256[$r0] = $r62r63
   addd $r0 = $r0, -256
   ;;
   lq.u $r62r63 = -256[$r1]
   addd $r1 = $r1, -256
   ;;
.Lbackwards_streaming_kernel_256:
.Lbackwards_streaming_remaining_256:
   sq -16[$r0] = $r32r33
   ;;
   sq -32[$r0] = $r34r35
   ;;
   sq -48[$r0] = $r36r37
   ;;
   sq -64[$r0] = $r38r39
   ;;
   sq -80[$r0] = $r40r41
   ;;
   sq -96[$r0] = $r42r43
   ;;
   sq -112[$r0] = $r44r45
   ;;
   sq -128[$r0] = $r46r47
   ;;
   sq -144[$r0] = $r48r49
   ;;
   sq -160[$r0] = $r50r51
   ;;
   sq -176[$r0] = $r52r53
   ;;
   sq -192[$r0] = $r54r55
   ;;
   sq -208[$r0] = $r56r57
   ;;
   sq -224[$r0] = $r58r59
   ;;
   sq -240[$r0] = $r60r61
   ;;
   sq -256[$r0] = $r62r63
   addd $r0 = $r0, -256
   ;;
.Lbackwards_remaining_256:
   andd $r11 = $r2, 16
   srld $r7 = $r2, 5
   ;;
   cb.deqz $r7? .Lbackwards_loop_remaining_32
   ;;
   loopdo $r7, .Lbackwards_loop_kernel_256
   ;;
   lo $r32r33r34r35 = -32[$r1]
   addd $r1 = $r1, -32
   addd $r2 = $r2, -32
   ;;
   so -32[$r0] = $r32r33r34r35
   addd $r0 = $r0, -32
   ;;
.Lbackwards_loop_kernel_256:
.Lbackwards_loop_remaining_32:
   andd $r10 = $r2, 8
   andd $r9 = $r2, 4
   cb.deqz $r11? .Lbackwards_loop_remaining_16
   ldcond q, "", .dnez, $r11, $r32r33, -16[$r1]
   ;;
   sq -16[$r0] = $r32r33
   addd $r1 = $r1, -16
   addd $r0 = $r0, -16
   ;;
.Lbackwards_loop_remaining_16:
   andd $r8 = $r2, 2
   andd $r7 = $r2, 1
   cb.deqz $r10? .Lbackwards_loop_remaining_8
   ldcond d, "", .dnez, $r10, $r32, -8[$r1]
   ;;
   sd -8[$r0] = $r32
   addd $r1 = $r1, -8
   addd $r0 = $r0, -8
   ;;
.Lbackwards_loop_remaining_8:
   cb.deqz $r9? .Lbackwards_loop_remaining_4
   ldcond wz, "", .dnez, $r9, $r32, -4[$r1]
   ;;
   sw -4[$r0] = $r32
   addd $r1 = $r1, -4
   addd $r0 = $r0, -4
   ;;
.Lbackwards_loop_remaining_4:
   cb.deqz $r8? .Lbackwards_loop_remaining_2
   ldcond hz, "", .dnez, $r8, $r32, -2[$r1]
   ;;
   sh -2[$r0] = $r32
   addd $r1 = $r1, -2
   addd $r0 = $r0, -2
   ;;
.Lbackwards_loop_remaining_2:
   ldcond bz, "", .dnez, $r7, $r32, -1[$r1]
   ;;
   addd $r12 = $r12, 64
   stcond b, .dnez, $r7, -1[$r0], $r32
   copyd $r0 = $r6
   ret
   ;;
#endif
.cfi_endproc

#ifdef MEMMOVE
.endp memmove
.type name, @function
.size memmove, . -memmove
#else
.endp memcpy
.type name, @function
.size memcpy, . -memcpy
#endif
