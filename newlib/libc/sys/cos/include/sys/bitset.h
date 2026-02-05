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

#ifndef _SYS_BITSET_H_
#define	_SYS_BITSET_H_

#include <sys/_types.h>
#include <sys/_bitcount.h>

#define	__bitset_mask(_s, n)						\
	(1L << ((__bitset_words((_s)) == 1) ?				\
	    (__size_t)(n) : ((n) % _BITSET_BITS)))

#define	__bitset_word(_s, n)						\
	((__bitset_words((_s)) == 1) ? 0 : ((n) / _BITSET_BITS))

#define	BIT_CLR(_s, n, p)						\
	((p)->__bits[__bitset_word(_s, n)] &= ~__bitset_mask((_s), (n)))

#define	BIT_COPY(_s, f, t)	(void)(*(t) = *(f))

#define	BIT_ISSET(_s, n, p)						\
	((((p)->__bits[__bitset_word(_s, n)] & __bitset_mask((_s), (n))) != 0))

#define	BIT_SET(_s, n, p)						\
	((p)->__bits[__bitset_word(_s, n)] |= __bitset_mask((_s), (n)))

#define	BIT_ZERO(_s, p) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(p)->__bits[__i] = 0L;					\
} while (0)

#define	BIT_FILL(_s, p) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(p)->__bits[__i] = -1L;					\
} while (0)

#define	BIT_SETOF(_s, n, p) do {					\
	BIT_ZERO(_s, p);						\
	(p)->__bits[__bitset_word(_s, n)] = __bitset_mask((_s), (n));	\
} while (0)

/* Is p empty. */
#define	BIT_EMPTY(_s, p) __extension__ ({				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		if ((p)->__bits[__i])					\
			break;						\
	__i == __bitset_words((_s));					\
})

/* Is p full set. */
#define	BIT_ISFULLSET(_s, p) __extension__ ({				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		if ((p)->__bits[__i] != (long)-1)			\
			break;						\
	__i == __bitset_words((_s));					\
})

/* Is c a subset of p. */
#define	BIT_SUBSET(_s, p, c) __extension__ ({				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		if (((c)->__bits[__i] &					\
		    (p)->__bits[__i]) !=				\
		    (c)->__bits[__i])					\
			break;						\
	__i == __bitset_words((_s));					\
})

/* Are there any common bits between b & c? */
#define	BIT_OVERLAP(_s, p, c) __extension__ ({				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		if (((c)->__bits[__i] &					\
		    (p)->__bits[__i]) != 0)				\
			break;						\
	__i != __bitset_words((_s));					\
})

/* Compare two sets, returns 0 if equal 1 otherwise. */
#define	BIT_CMP(_s, p, c) __extension__ ({				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		if (((c)->__bits[__i] !=				\
		    (p)->__bits[__i]))					\
			break;						\
	__i != __bitset_words((_s));					\
})

#define	BIT_OR(_s, d, s) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] |= (s)->__bits[__i];			\
} while (0)

#define	BIT_OR2(_s, d, s1, s2) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] = (s1)->__bits[__i] | (s2)->__bits[__i];\
} while (0)

#define	BIT_AND(_s, d, s) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] &= (s)->__bits[__i];			\
} while (0)

#define	BIT_AND2(_s, d, s1, s2) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] = (s1)->__bits[__i] & (s2)->__bits[__i];\
} while (0)

#define	BIT_NAND(_s, d, s) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] &= ~(s)->__bits[__i];			\
} while (0)

#define	BIT_NAND2(_s, d, s1, s2) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] = (s1)->__bits[__i] & ~(s2)->__bits[__i];\
} while (0)

#define	BIT_XOR(_s, d, s) do {						\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] ^= (s)->__bits[__i];			\
} while (0)

#define	BIT_XOR2(_s, d, s1, s2) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		(d)->__bits[__i] = (s1)->__bits[__i] ^ (s2)->__bits[__i];\
} while (0)

#define	BIT_CLR_ATOMIC(_s, n, p)					\
	atomic_clear_long(&(p)->__bits[__bitset_word(_s, n)],		\
	    __bitset_mask((_s), n))

#define	BIT_SET_ATOMIC(_s, n, p)					\
	atomic_set_long(&(p)->__bits[__bitset_word(_s, n)],		\
	    __bitset_mask((_s), n))

#define	BIT_SET_ATOMIC_ACQ(_s, n, p)					\
	atomic_set_acq_long(&(p)->__bits[__bitset_word(_s, n)],		\
	    __bitset_mask((_s), n))

/* Convenience functions catering special cases. */
#define	BIT_AND_ATOMIC(_s, d, s) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		atomic_clear_long(&(d)->__bits[__i],			\
		    ~(s)->__bits[__i]);					\
} while (0)

#define	BIT_OR_ATOMIC(_s, d, s) do {					\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		atomic_set_long(&(d)->__bits[__i],			\
		    (s)->__bits[__i]);					\
} while (0)

#define	BIT_COPY_STORE_REL(_s, f, t) do {				\
	__size_t __i;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		atomic_store_rel_long(&(t)->__bits[__i],		\
		    (f)->__bits[__i]);					\
} while (0)

#define	BIT_FFS(_s, p) __extension__ ({					\
	__size_t __i;							\
	int __bit;							\
									\
	__bit = 0;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++) {		\
		if ((p)->__bits[__i] != 0) {				\
			__bit = ffsl((p)->__bits[__i]);			\
			__bit += __i * _BITSET_BITS;			\
			break;						\
		}							\
	}								\
	__bit;								\
})

#define	BIT_FLS(_s, p) __extension__ ({					\
	__size_t __i;							\
	int __bit;							\
									\
	__bit = 0;							\
	for (__i = __bitset_words((_s)); __i > 0; __i--) {		\
		if ((p)->__bits[__i - 1] != 0) {			\
			__bit = flsl((p)->__bits[__i - 1]);		\
			__bit += (__i - 1) * _BITSET_BITS;		\
			break;						\
		}							\
	}								\
	__bit;								\
})

#define	BIT_COUNT(_s, p) __extension__ ({				\
	__size_t __i;							\
	int __count;							\
									\
	__count = 0;							\
	for (__i = 0; __i < __bitset_words((_s)); __i++)		\
		__count += __bitcountl((p)->__bits[__i]);		\
	__count;							\
})

#define	BITSET_T_INITIALIZER(x)						\
	{ .__bits = { x } }

#define	BITSET_FSET(n)							\
	[ 0 ... ((n) - 1) ] = (-1L)

/*
 * Dynamically allocate a bitset.
 */
#define BITSET_ALLOC(_s, mt, mf)					\
	malloc(__bitset_words(_s) * sizeof(long), mt, (mf))

#endif /* !_SYS_BITSET_H_ */
