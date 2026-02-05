/*
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

/* This header file contains various common macros and types and KVX builtins
   for use in KVX optimized implementations of libc functions */

#ifndef KVX_DEFINES_H
#define KVX_DEFINES_H

#include <stdint.h>


/* Helpers */

/* Check for a null terminator byte (64bits). Used in str functions.
   From https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
   The subexpression (~(X) & 0x8080808080808080) is replaced to the equivalent
   ~((X) | 0x7f7f7f7f7f7f7f7f) for better bundling. */
#define DETECTNULL(X) \
    (((X) - 0x0101010101010101) & ~((X) | 0x7f7f7f7f7f7f7f7f))

/* Nonzero if X is not aligned on a "type" boundary */
#define UNALIGNED(type, X) ((uintptr_t)X & (sizeof(type) - 1))
/* Nonzero if either X or Y is not aligned on a "type" boundary */
#define UNALIGNED2(type, X, Y) (UNALIGNED(type, X) | UNALIGNED(type, Y))
/* Create a mask to round down an address to a "type" boundary */
#define ALIGN_MASK(type) (~(sizeof(type) - 1))


/* Smallest page size */
#define PAGE_SIZE (4096)
/* Zero if X will cross pages on load of "type" */
#define PAGE_CROSS_SAFE(type, X) \
    (((uintptr_t)X & (PAGE_SIZE - sizeof(type))) ^ (PAGE_SIZE - sizeof(type)))
/* Zero if either X or Y will cross pages on load of "type" */
#define PAGE_CROSS_SAFE2(type, X, Y) \
    (PAGE_CROSS_SAFE(type, X) | PAGE_CROSS_SAFE(type, Y))

#define unlikely(cond) __builtin_expect(!!(cond), 0)
#define likely(cond) __builtin_expect(!!(cond), 1)

/* Vector types */
typedef uint64_t uint64x2_t __attribute__((vector_size(2 * sizeof(uint64_t))));
typedef uint64_t uint64x4_t __attribute__((vector_size(4 * sizeof(uint64_t))));


/* KVX builtins */

/* Count trailing zeroes (64bits) */
#define cnt_trailzero(a) __builtin_kvx_bitcntd(a, ".tz")


#endif /* KVX_DEFINES_H */
