# -*- Makefile -*- vim:ft=make:
AM_CPPFLAGS += $(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS) -I$(newlib_basedir)/libc/search

libc_a_SOURCES += \
  %D%/setjmp.S \
  %D%/memset.S \
  %D%/memcmp.S \
  %D%/memcpy.S \
  %D%/memmove.S \
  %D%/rpmatch.c \
  %D%/arc4random.c \
  %D%/arc4random_uniform.c \
  %D%/hash_page.c
