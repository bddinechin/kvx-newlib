#ifdef USING_SIM_SPECS

// Gdb simulator requires that sbrk be implemented without a syscall.
extern char _end[];                /* _end is set in the linker command file */
char *heap_ptr;

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
char *
_sbrk (nbytes)
     int nbytes;
{
  char        *base;

  if (!heap_ptr)
    heap_ptr = (char *)&_end;
  base = heap_ptr;
  heap_ptr += nbytes;

  return base;
}

#elif defined(__CLUSTER_OS__)

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <hal/cos_link_configs.h>

static char * _heap_ptr = NULL; /* calulated using symbols from the linker script: _heap_start and _heap_end */
static char *_end_of_allocatable_mem = NULL;

void *_sbrk(ptrdiff_t nbytes) {
  /* Arguments are:
     - number of bytes of memory to allocate
     This call atomically checks for overflow and increments the heap
     pointer if all is ok.
     The memory layout has the heap area starting above the user code and
     static data and growing up, and the stack starting in high memory
     (below kernel stack and environment, etc.) and growing down.
  */

  void * old_heap_ptr;
  void * returnCode;
  unsigned long initial_heap = 0;

  if (_heap_ptr == NULL) {
      unsigned long pagesz = getpagesize();

      /*      _heap_ptr = &_end; */
      /*
       * Align heap start on a page boundary like further acllocations
       * in mallocr.c. This avoids differences in cache behavior due to
       * small increases of sections size.
       */
      _heap_ptr = (char *) ((((unsigned long) &_heap_start) + (pagesz - 1)) &
			    ~(pagesz - 1));
      initial_heap = (unsigned long) (void *) _heap_ptr;
      _end_of_allocatable_mem = (char *) &_heap_end;
  }
  old_heap_ptr = _heap_ptr ;

  /* Check overflows */
  if (nbytes >= 0) {
      if (! (nbytes <= ((unsigned long) (void *) _end_of_allocatable_mem -
			(unsigned long) (void *) _heap_ptr))) {
	  goto bad;
      }
  } else {
      if (! (abs(nbytes) <= ((unsigned long) old_heap_ptr - initial_heap))) {
	  goto bad;
      }
  }

  _heap_ptr += nbytes;
  returnCode = old_heap_ptr;

  return returnCode;

bad:
#ifdef REENTRANT_SYSCALLS_PROVIDED
  /* errno is set in the reent structure by _sbrk_r() */
#else /* REENTRANT_SYSCALLS_PROVIDED */
  errno = ENOMEM;
#endif
  return ((void *) -1);
}

#else

// QEMU uses a syscall.
#include <machine/syscall.h>
#include <sys/types.h>
#include "internal_syscall.h"

/* Increase program data space. As malloc and related functions depend
   on this, it is useful to have a working implementation. The following
   is suggested by the newlib docs and suffices for a standalone
   system.  */
void *
_sbrk(ptrdiff_t incr)
{
  static unsigned long heap_end;

  if (heap_end == 0)
    {
      long brk = __internal_syscall (SYS_brk, 1, 0, 0, 0, 0, 0, 0);
      if (brk == -1)
        return (void *)__syscall_error (-ENOMEM);
      heap_end = brk;
    }

  if (__internal_syscall (SYS_brk, 1, heap_end + incr, 0, 0, 0, 0, 0) != heap_end + incr)
    return (void *)__syscall_error (-ENOMEM);

  heap_end += incr;
  return (void *)(heap_end - incr);
}
#endif
