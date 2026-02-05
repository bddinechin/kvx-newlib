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

/**
 * L1 cache interface
 */

/* L1 cache defines */
#define _KVX_DCACHE_LINE_SIZE    ((64*8)/8)
#define _KVX_DCACHE_SIZE (128*1024)

#define _KVX_ICACHE_LINE_SIZE    ((64*8)/8)
#define _KVX_ICACHE_SIZE (4*8*1024)

/**
 * \fn void __libc_recursive_initlock_base(_LOCK_T *lock)
 * \brief Initialize recursive lock
 * \param lock Pointer to recursive lock struct
 * \warning Cache line containing lock struct is flushed
 */
static void __libc_recursive_initlock_base(_LOCK_T *lock)
{
  *(volatile uint32_t *)&(lock->owner) = _KVX_RECURSIVE_NO_OWNER;
  *(volatile uint32_t *)&(lock->counter) = 0;
}

/**
 * \fn int __libc_recursive_trylock_base(_LOCK_T *lock,
 *                                     uint64_t myself)
 * \brief Try to acquire recursive lock
 * This function tries to acquire recursive lock. It does not perform any
 * cache operations.
 * \param lock Pointer to recursive lock struct
 * \param myself My owner value. WARNING 0x0 means unlocked
 * \return 1 if lock is acquired, else 0
 * \warning The cache line containing the lock counter is flushed
 */
static int __libc_recursive_trylock_base(_LOCK_T *lock, uint32_t myself)
{
  uint32_t swap_success;

  swap_success = __builtin_kvx_acswapw(&(lock->owner), myself, _KVX_RECURSIVE_NO_OWNER);
  if (swap_success == 1
      || __builtin_kvx_lwz(&lock->owner, ".u", 0) == myself) {
    uint64_t counter = __builtin_kvx_lwz(&(lock->counter), ".u", 0);

    counter += 1;
    *(volatile uint32_t *)&(lock->counter) = counter;
    return 1;
  } else {
    return 0;
  }
}

/**
 * \fn void __libc_recursive_unlock_base(_LOCK_T *lock,
 *                                     uint64_t myself)
 * \brief Release recursive lock
 * This function unlocks an recursive lock. It does not perform any
 * cache oeprations.
 * \param lock Pointer to recursive lock struct
 * \param myself My owner value. WARNING 0x0 means unlocked
 */
static int __libc_recursive_unlock_base(_LOCK_T *lock, uint64_t myself)
{
  uint32_t counter = __builtin_kvx_lwz((void *)&(lock->counter), ".u", 0);
  uint32_t owner = __builtin_kvx_lwz((void *)&(lock->owner), ".u", 0);

  if (owner != myself) {
    return 0;
  }

  counter -= 1;
  *(volatile uint32_t *)&(lock->counter) = counter;
  if (counter == 0) {
    /* Unlock */
    *(volatile uint32_t *)&(lock->owner) = _KVX_RECURSIVE_NO_OWNER;
  }
  return 1;
}

/* Defined in bsp.c in libgloss. */
extern int __kvx_get_cpu_id(void);

static inline unsigned long long libc_get_id(void)
{
  return __kvx_get_cpu_id() + 1;
}

static inline void libc_cache_flush(void)
{
  /* Full memory barrier */
  __builtin_kvx_d1inval();
  __builtin_kvx_fence();
}

/* All these entries are declared in newlib/libc/sys/kvx-mbr/include/sys/lock.h */

void __libc_lock_init(_LOCK_T *lock) __attribute__((alias ("__libc_lock_init_recursive")));
void __libc_lock_init_recursive(_LOCK_T *lock)
{
  if(lock == NULL) {
    /* Unable to allocate memory: trig a trap opcode. */
    asm("errop\n;;");
  }
  __libc_recursive_initlock_base(lock);
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
  res = __libc_recursive_trylock_base(lock, myself);
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
  __libc_recursive_unlock_base(lock, myself);
}
