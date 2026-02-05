/*
 * Copyright 2021
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

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Protection attributes
 */
#define PROT_NONE 0x00 /* no permissions */
#define PROT_READ 0x01 /* pages can be read */
#define PROT_WRITE 0x02 /* pages can be written */
#define PROT_EXEC 0x04 /* pages can be executed */

/*
 * Map flags attributes
 */
#define MAP_SHARED 0x0001 /* share changes */
#define MAP_PRIVATE 0x0002 /* changes are private */
#define MAP_FIXED 0x0010 /* map addr must be exactly as requested */
#define MAP_ANONYMOUS 0x020 /* no banked by any file */
#define MAP_ANON MAP_ANONYMOUS /* no banked by any file */
#define MAP_HUGETLB 0x40000 /* huge page mapping */
#define MAP_POPULATE 0x08000 /* preload pagetables */
#define MAP_FAILED ((void *)-1) /* invalid address */

/*
 * Sync flags
 */
#define MS_SYNC 0x0000 /* msync synchronously */
#define MS_ASYNC 0x0001 /* return immediately */
#define MS_INVALIDATE 0x0002 /* invalidate all cached data */

/*
 * Madvise flags
 */
#define MADV_NORMAL 0 /* no further special treatment */
#define MADV_RANDOM 1 /* expect random page references */
#define MADV_SEQUENTIAL 2 /* expect sequential page references */
#define MADV_WILLNEED 3 /* will need these pages */
#define MADV_DONTNEED 4 /* dont need these pages */
#define MADV_FREE 5 /* dont need these pages, and junk contents */
#define MADV_NOSYNC 6 /* try to avoid flushes to physical media */
#define MADV_AUTOSYNC 7 /* revert to default flushing strategy */
#define MADV_NOCORE 8 /* do not include these pages in a core file */
#define MADV_CORE 9 /* revert to including pages in a core file */
#define MADV_PROTECT 10 /* protect process from pageout kill */

/*
 * POSIX madvice flags
 */
#define POSIX_MADV_NORMAL _MADV_NORMAL
#define POSIX_MADV_RANDOM _MADV_RANDOM
#define POSIX_MADV_SEQUENTIAL _MADV_SEQUENTIAL
#define POSIX_MADV_WILLNEED _MADV_WILLNEED
#define POSIX_MADV_DONTNEED _MADV_DONTNEED

/*
 * Mlock flags
 */
#define MCL_CURRENT 0x0001 /* Lock only current memory */
#define MCL_FUTURE 0x0002 /* Lock all future memory as well */
#define MCL_ONFAULT 0x0004 /* Map when they are faulted in */

/*
 * mman prototypes
 */
void *mmap(void *, size_t, int, int, int, off_t);
int munmap(void *, size_t);

int mlockall(int);
int munlockall(void);

int shm_open(const char *, int, mode_t);
int shm_unlink(const char *);

int mlock(const void *, size_t);
int munlock(const void *, size_t);

int madvise(void *, size_t, int);
int posix_madvise(void *, size_t, int);

int mprotect(const void *, size_t, int);
int msync(void *, size_t, int);

#ifdef __cplusplus
}
#endif

#endif /* !_SYS_MMAN_H_ */
