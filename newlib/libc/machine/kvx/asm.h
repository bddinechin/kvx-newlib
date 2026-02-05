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

#ifdef __STDC__
# define _C_LABEL(x)    _ ## x
#else
# define _C_LABEL(x)    _/**/x
#endif
#define _ASM_LABEL(x)   x

#define _ENTRY(name)	\
	.section .text; .align 8; .proc name; .global name; name:

#define ENTRY(name)	\
	_ENTRY(_C_LABEL(name))

#if defined(__ASSEMBLER__)
.macro stcond size, cond, reg, dst, src
#if defined(__kvxarch_kv3_1) || defined(__kvxarch_kv3_2)
s\size\cond \reg \()? \dst \()= \src
#elif defined(__kvxarch_kv4_1)
guard\cond \reg \()? s\size \dst \()= \src
#else
#error Unknown architecture
#endif
.endm

.macro ldcond size, cached, cond, reg, dst, src
#if defined(__kvxarch_kv3_1) || defined(__kvxarch_kv3_2)
l\size\cached\cond \reg \()? \dst \()= \src
#elif defined(__kvxarch_kv4_1)
guard\cond \reg \()? l\size\cached \dst \()= \src
#else
#error Unknown architecture
#endif
.endm
#endif
