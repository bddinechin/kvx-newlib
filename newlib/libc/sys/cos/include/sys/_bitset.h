/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2008, Jeffrey Roberson <jeff@freebsd.org>
 * All rights reserved.
 *
 * Copyright (c) 2008 Nokia Corporation
 * All rights reserved.
 *
 * Copyright 2022
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

#ifndef _SYS__BITSET_H_
#define	_SYS__BITSET_H_

/*
 * Macros addressing word and bit within it, tuned to make compiler
 * optimize cases when SETSIZE fits into single machine word.
 */
#define	_BITSET_BITS		(sizeof(long) * 8)

#define	__howmany(x, y)	(((x) + ((y) - 1)) / (y))

#define	__bitset_words(_s)	(__howmany(_s, _BITSET_BITS))

#define	BITSET_DEFINE(t, _s)						\
struct t {								\
        long    __bits[__bitset_words((_s))];				\
}

/*
 * Helper to declare a bitset without it's size being a constant.
 *
 * Sadly we cannot declare a bitset struct with '__bits[]', because it's
 * the only member of the struct and the compiler complains.
 */
#define BITSET_DEFINE_VAR(t)	BITSET_DEFINE(t, 1)

/*
 * Define a default type that can be used while manually specifying size
 * to every call.
 */
BITSET_DEFINE(___bitset, 1);

#endif /* !_SYS__BITSET_H_ */
