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

#ifndef __SYS_LOCK_H__
#define __SYS_LOCK_H__

#include <stdint.h>
#include <mppa_bare_runtime/kvx/atomic.h>
#include <mppa_bare_runtime/kvx/cache.h>

/*
 * Lock routines for mppa bare runtime toolchain.
 */

#ifndef _KVX_RECURSIVE_NO_OWNER
#define _KVX_RECURSIVE_NO_OWNER 0x0UL
#endif /* _KVX_RECURSIVE_NO_OWNER  */

struct __lock {
  __kvx_recursive_lock_t lock;
} __attribute__ ((aligned (_KVX_DCACHE_LINE_SIZE)));

typedef struct __lock _LOCK_T;
#define _LOCK_RECURSIVE_T _LOCK_T

#define __LOCK_INIT(class,lock) class _LOCK_T lock = {0}
#define __LOCK_INIT_RECURSIVE(class,lock) __LOCK_INIT(class,lock)

extern void __libc_lock_init(_LOCK_T *lock);
extern void __libc_lock_init_recursive(_LOCK_RECURSIVE_T *lock);
extern void __libc_lock_close(_LOCK_T *lock);
extern void __libc_lock_close_recursive(_LOCK_RECURSIVE_T *lock);
extern void __libc_lock_acquire(_LOCK_T *lock);
extern void __libc_lock_acquire_recursive(_LOCK_RECURSIVE_T *lock);
extern void __libc_lock_release(_LOCK_T *lock);
extern void __libc_lock_release_recursive(_LOCK_RECURSIVE_T *lock);

/* Returns 0 for success and non-zero for failure */
extern int __libc_lock_try_acquire(_LOCK_T *lock);
extern int __libc_lock_try_acquire_recursive(_LOCK_RECURSIVE_T *lock);

#define __lock_init(lock) \
    __libc_lock_init(&(lock))
#define __lock_init_recursive(lock) \
    __libc_lock_init_recursive(&(lock))
#define __lock_close(lock) \
    __libc_lock_close(&(lock))
#define __lock_close_recursive(lock) \
    __libc_lock_close_recursive(&(lock))
#define __lock_acquire(lock) \
    __libc_lock_acquire(&(lock))
#define __lock_acquire_recursive(lock) \
    __libc_lock_acquire_recursive(&(lock))
#define __lock_try_acquire(lock) \
    __libc_lock_try_acquire(&(lock))
#define __lock_try_acquire_recursive(lock) \
    __libc_lock_try_acquire_recursive(&(lock))
#define __lock_release(lock) \
    __libc_lock_release(&(lock))
#define __lock_release_recursive(lock) \
    __libc_lock_release_recursive(&(lock))

#endif /* __SYS_LOCK_H__ */
