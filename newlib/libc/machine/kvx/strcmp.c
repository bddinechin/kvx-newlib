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

#include "kvx_defines.h"


int
strcmp(const char *s1, const char *s2) {
    uint64_t has_null0;
    uint64_t has_null1;

    uint64x2_t *a1;
    uint64x2_t *a2;

    uint64x2_t vv1;
    uint64x2_t vv2;

    /* Compare strings 128 bits at a time until detecting a difference
        or a null terminator */
    a1 = (uint64x2_t *)s1;
    a2 = (uint64x2_t *)s2;
    do {
        /* Check for page crossing when doing unaligned loads */
        if (!PAGE_CROSS_SAFE2(uint64x2_t, a1, a2)) {
            char *s1_tmp = (char *)a1;
            char *s2_tmp = (char *)a2;

            /* Compare 1 byte at a time until the page has been crossed */
            do {
                unsigned char c1 = *s1_tmp++;
                unsigned char c2 = *s2_tmp++;
                if (c1 == '\0' || c1 != c2) {
                    return c1 - c2;
                }
            } while (!PAGE_CROSS_SAFE2(uint64x2_t, s1_tmp, s2_tmp));

            /* Update pointers */
            a1 = (uint64x2_t *)s1_tmp;
            a2 = (uint64x2_t *)s2_tmp;
        }

        vv1 = *a1++;
        vv2 = *a2++;
        has_null0 = DETECTNULL(vv1[0]);
        has_null1 = DETECTNULL(vv1[1]);
    } while ((vv1[0] == vv2[0] && vv1[1] == vv2[1]) &&
            !(has_null0 || has_null1));

    /* The strings are equal after iteration, return zero */
    if (vv1[0] == vv2[0] && vv1[1] == vv2[1] && (has_null0 || has_null1)) {
        return 0;
    }

    /* Edge case: The null terminator is stored in the last byte of
       the first 64 bits */
    if (vv1[0] == vv2[0] && has_null0) {
        return 0;
    }

    /* Otherwise count the trailing zeroes to determine where the
       first different bit is */
    uint64_t trail = cnt_trailzero((vv1[0] ^ vv2[0]) | has_null0);
    uint64_t v1 = vv1[0];
    uint64_t v2 = vv2[0];

    /* If the first 64 bits are equal, check the last 64 bits */
    if (vv1[0] == vv2[0]) {
        trail = cnt_trailzero((vv1[1] ^ vv2[1]) | has_null1);
        v1 = vv1[1];
        v2 = vv2[1];
    }

    /* Round to lower multiple of 8 to get the different byte */
    trail &= ~UINT64_C(7);

    const unsigned char c1 = v1 >> trail;
    const unsigned char c2 = v2 >> trail;

    return c1 - c2;
}
