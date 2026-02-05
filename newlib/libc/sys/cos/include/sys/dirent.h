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

#ifndef _SYS_DIRENT_H
# define _SYS_DIRENT_H

typedef struct __dirdesc {
	int	dd_fd;
	long	dd_loc;
	long	dd_size;
	char	*dd_buf;
	int	dd_len;
	long	dd_seek;
} DIR;

# define __dirfd(dp)	((dp)->dd_fd)

#include <sys/types.h>

#undef  MAXNAMLEN	/* from unistd.h */
#define MAXNAMLEN	512

#define d_ino	d_fileno	/* compatibility */

/* Use for getdents64 */
struct dirent {
	unsigned long long d_ino;
	unsigned long long d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[MAXNAMLEN];
};

/* File types for `d_type'.  */
enum
{
	DT_UNKNOWN = 0,
# define DT_UNKNOWN     DT_UNKNOWN
	DT_FIFO = 1,
# define DT_FIFO        DT_FIFO
	DT_CHR = 2,
# define DT_CHR         DT_CHR
	DT_DIR = 4,
# define DT_DIR         DT_DIR
	DT_BLK = 6,
# define DT_BLK         DT_BLK
	DT_REG = 8,
# define DT_REG         DT_REG
	DT_LNK = 10,
# define DT_LNK         DT_LNK
	DT_SOCK = 12,
# define DT_SOCK        DT_SOCK
	DT_WHT = 14
# define DT_WHT         DT_WHT
};

/* Convert between stat structure types and directory types.  */
# define IFTODT(mode)   (((mode) & 0170000) >> 12)
# define DTTOIF(dirtype)        ((dirtype) << 12)

#endif
