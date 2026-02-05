/*
 * Copyright 2005 Shaun Jackman
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
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


#include <libgen.h>
#include <string.h>

char*
basename (char *path)
{
	char *p;

	if (path == NULL || *path == '\0')
		return ".";

	p = path + strlen(path) - 1;

	while (*p == '/') {
		if (p == path)
			return path;
		*p-- = '\0';
	}

	while (p >= path && *p != '/')
		p--;

	return p + 1;
}

char *
dirname (char *path)
{
	char *p;

	if (path == NULL || *path == '\0')
		return ".";

	p = path + strlen(path) - 1;

	while (*p == '/') {
		if (p == path)
			return path;
		*p-- = '\0';
	}

	while (p >= path && *p != '/')
		p--;

	return p < path ? "." :
		p == path ? "/" :
		(*p = '\0', path);
}
