/* This file has been modified or added by Kalray, SA. 2010 */

/* Implementation of non-OS memory allocation */

#include <unistd.h>
#include <errno.h>
#include <hal/cos_link_configs.h>

extern void *_sbrk (ptrdiff_t);

static char * _heap_ptr = NULL; /* calulated using symbols from the linker script: _heap_start and _heap_end */
static char *_end_of_allocatable_mem = NULL;

static int abs(int i)
{
	return (i < 0) ? -i : i;
}

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
      initial_heap = (unsigned long) _heap_ptr;
      _end_of_allocatable_mem = (char *) &_heap_end;
  }
  old_heap_ptr = _heap_ptr ;

  /* Check overflows */
  if (nbytes >= 0) {
      if (! (nbytes <= ((unsigned long) _end_of_allocatable_mem -
			(unsigned long) _heap_ptr))) {
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

void *sbrk(ptrdiff_t nbytes) __attribute__ ((weak, alias ("_sbrk")));
