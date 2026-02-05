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

#include <stdlib.h>
#include <sys/lock.h>

#include <hal/cos_atomic.h>
#include <hal/cos_cpu.h>

static inline unsigned long long libc_get_id(void)
{
  return __cos_get_cluster_id() * (_COS_NB_PE + _COS_NB_RM + 1) + __cos_get_cpu_id() + 1;
}

static inline void libc_cache_flush(void)
{
  __cos_mb();
}

void __libc_lock_init(_LOCK_T *lock) __attribute__((alias ("__libc_lock_init_recursive")));
void __libc_lock_init_recursive(_LOCK_T *lock)
{
  if(lock == NULL) {
    /* Unable to allocate memory: trig a exception */
    __builtin_trap();
  }
  __cos_recursive_initlock_base(&(lock->lock));
}

void __libc_lock_close(_LOCK_T *lock) __attribute__((alias ("__libc_lock_close_recursive")));
void __libc_lock_close_recursive(_LOCK_T *lock)
{
}

void __libc_lock_acquire(_LOCK_T *lock) __attribute__((alias ("__libc_lock_acquire_recursive")));
void __libc_lock_acquire_recursive(_LOCK_T *lock)
{
  unsigned int backoff;
  while (__libc_lock_try_acquire_recursive(lock)) {
    volatile int i;
    backoff = libc_get_id ();
    for (i = 0; i < backoff; i++);
    backoff += 13;
  }
  /* invalidate cache when we get lock */
  libc_cache_flush();
}

int __libc_lock_try_acquire(_LOCK_T *lock) __attribute__((alias ("__libc_lock_try_acquire_recursive")));
int __libc_lock_try_acquire_recursive(_LOCK_T *lock)
{
  unsigned long long myself;
  unsigned long long owner;
  int res;

  myself = libc_get_id();
  res = __cos_recursive_trylock_base(&(lock->lock), myself);
  if (res == 1)
    libc_cache_flush();
  return !res;
}

void __libc_lock_release(_LOCK_T *lock) __attribute__((alias ("__libc_lock_release_recursive")));
void __libc_lock_release_recursive(_LOCK_T *lock)
{
  unsigned long long myself;
  myself = libc_get_id();
  libc_cache_flush();
  __cos_recursive_unlock_base(&(lock->lock), myself);
}
