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

#include <stddef.h>

size_t
strlen(const char *str) {
    uint64x4_t *a256 = (uint64x4_t *)str;
    uint64x4_t v256;

    /* Do a 256bits unaligned prepass first if no page cross will occur */
    if (PAGE_CROSS_SAFE(uint64x4_t, a256)) {
        v256 = *a256;
        size_t len = ((char *)a256) - str;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

        /* Get the offset of a potential null terminator in each 64 bits */
        uint64_t offset0 = cnt_trailzero(has_null0);
        uint64_t offset1 = cnt_trailzero(has_null1);
        uint64_t offset2 = cnt_trailzero(has_null2);
        uint64_t offset3 = cnt_trailzero(has_null3);

        /* Return the length of the string if a null terminator was found */
        if (has_null0) {
            return len + (offset0 / 8);
        }
        if (has_null1) {
            return len + (offset1 / 8) + 8;
        }
        if (has_null2) {
            return len + (offset2 / 8) + 16;
        }
        if (has_null3) {
            return len + (offset3 / 8) + 24;
        }

        a256++;

        /* Round down address to 32 bytes alignment, which means we will be
            checking some previous bytes twice, but remove page cross check
            from the main loop */
        a256 = (uint64x4_t *)((uintptr_t)a256 & ALIGN_MASK(uint64x4_t));

    /* Page cross on the first load is rare, so read 1 byte at a time until
       the pointer is aligned.
       This could be improved by loading from a lower 32 bytes aligned address
       and mask preceding bytes to 0x7F (non '\0'), then go to main loop */
    } else {
        char *a8 = (char *)a256;

        /* Iterate 1 byte at a time until the page has been crossed */
        do {
            char c = *a8++;
            if (c == '\0') {
                return --a8 - str;
            }
        } while(!PAGE_CROSS_SAFE(uint64x4_t, a8));

        /* Update pointer */
        a256 = (uint64x4_t *)a8;
    }


    /* Main aligned loop */
    do {
        v256 = *a256;
        size_t len = ((char *)a256) - str;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

        /* Get the offset of a potential null terminator in each 64 bits */
        uint64_t offset0 = cnt_trailzero(has_null0);
        uint64_t offset1 = cnt_trailzero(has_null1);
        uint64_t offset2 = cnt_trailzero(has_null2);
        uint64_t offset3 = cnt_trailzero(has_null3);

        /* Return the length of the string when a null terminator was found */
        if (unlikely(has_null0)) {
            return len + (offset0 / 8);
        }
        if (unlikely(has_null1)) {
            return len + (offset1 / 8) + 8;
        }
        if (unlikely(has_null2)) {
            return len + (offset2 / 8) + 16;
        }
        if (unlikely(has_null3)) {
            return len + (offset3 / 8) + 24;
        }

        a256++;
    } while (1);
}
