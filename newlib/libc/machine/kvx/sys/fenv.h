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

/*
 *  This file provides sys/fenv.h for the Coolidge processor.
 *
 *  Each core of the Coolidge processor have a coprocessor. They share
 *  the CS register but have distinct bit-fields for their
 *  floating-point environment.  This implementation synchronizes them
 *  in such a way that they cannot be managed separately.
 */

#ifndef _SYS_FENV_H
#define _SYS_FENV_H

/* Compute Status ($cs) register contains the following bit-fields for
 *   floating-point execption flags.
 *
 *   Bit-field Condition of the IEEE 754 binary floating-point standard
 *   --------- --------------------------------------------------------
 *   IO        Invalid Operation
 *   DZ        Divide by Zero
 *   OV        Overflow
 *   UN        Underflow
 *   IN        Inexact
 *   XIO       Invalid Operation (coprocessor)
 *   XDZ       Divide by Zero (coprocessor)
 *   XOV       Overflow (coprocessor)
 *   XUN       Underflow (coprocessor)
 *   XIN       Inexact (coprocessor)
 */

#define _FE_INVALID   0x02
#define _FE_DIVBYZERO 0x04
#define _FE_OVERFLOW  0x08
#define _FE_UNDERFLOW 0x10
#define _FE_INEXACT   0x20

#define _FE_X_INVALID   0x0200
#define _FE_X_DIVBYZERO 0x0400
#define _FE_X_OVERFLOW  0x0800
#define _FE_X_UNDERFLOW 0x1000
#define _FE_X_INEXACT   0x2000

#define FE_INVALID   (_FE_INVALID   | _FE_X_INVALID)
#define FE_DIVBYZERO (_FE_DIVBYZERO | _FE_X_DIVBYZERO)
#define FE_OVERFLOW  (_FE_OVERFLOW  | _FE_X_OVERFLOW)
#define FE_UNDERFLOW (_FE_UNDERFLOW | _FE_X_UNDERFLOW)
#define FE_INEXACT   (_FE_INEXACT   | _FE_X_INEXACT)

#define FE_ALL_EXCEPT (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT)

/* Compute Status ($cs) register contains the following bit-fields for
 * floating-point rounding modes.
 *
 * Following table describes both the RM and XRM (coproc) bit-fields.
 */

#if defined(__KV4__)

/*
 * Value Rounding Mode of the IEEE 754 binary floating-point standard
 * ----- ------------------------------------------------------------
 * 0b00  to nearest even
 * 0b01  toward zero
 * 0b10  toward -inf
 * 0b11  toward +inf
 */

#define _FE_TONEAREST  0
#define _FE_TOWARDZERO 1
#define _FE_DOWNWARD   2
#define _FE_UPWARD     3

#define _FE_X_TONEAREST  0
#define _FE_X_TOWARDZERO 1
#define _FE_X_DOWNWARD   2
#define _FE_X_UPWARD     3

#elif defined(__KV3__)

/*
 * Value Rounding Mode of the IEEE 754 binary floating-point standard
 * ----- ------------------------------------------------------------
 * 0b00  to nearest even
 * 0b01  toward +inf
 * 0b10  toward -inf
 * 0b11  toward zero
 */

#define _FE_TONEAREST  0
#define _FE_UPWARD     1
#define _FE_DOWNWARD   2
#define _FE_TOWARDZERO 3

#define _FE_X_TONEAREST  0
#define _FE_X_UPWARD     1
#define _FE_X_DOWNWARD   2
#define _FE_X_TOWARDZERO 3

#endif//__KV3__||__KV4__

#define FE_TONEAREST  ((_FE_TONEAREST  << 16) | (_FE_X_TONEAREST  << 20))
#define FE_UPWARD     ((_FE_UPWARD     << 16) | (_FE_X_UPWARD     << 20))
#define FE_DOWNWARD   ((_FE_DOWNWARD   << 16) | (_FE_X_DOWNWARD   << 20))
#define FE_TOWARDZERO ((_FE_TOWARDZERO << 16) | (_FE_X_TOWARDZERO << 20))

#define FE_RND_MASK   ((3              << 16) | (3                << 20))

/* The type representing all floating-point status flags collectively.
 * The environment is simply a copy from the FPU related bits in the
 * CS register, but can be improved in the future.
 */
typedef unsigned int fexcept_t;
/* The type representing the entire floating-point environment.  The
 * environment is simply a copy from the FPU related bits in the CS
 * register.
 */
typedef unsigned int fenv_t;

extern const fenv_t *_fe_dfl_env;
#define FE_DFL_ENV   _fe_dfl_env

#endif /* sys/fenv.h */
