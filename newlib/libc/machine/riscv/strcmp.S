.cfi_sections .debug_frame
.text
.global strcmp
.type   strcmp, @function
/* Parameters:
	$a0: a pointer
	$a1: b pointer
	This Algorithm performs speculative loads up to
	the end of the page containing the end of each string.
	May cause illegal memory access warnings.
*/
strcmp:
	.cfi_startproc
/* Save registers */
	addi    sp,sp,-80
	sd      ra,72(sp)
	sd      s0,64(sp)
	addi    s0,sp,80
	sd      a0,-56(s0)
	sd      a1,-64(s0)
	sd      s6,-72(s0)
	sd      s2,-48(s0)
	sd      s3,-40(s0)
	sd      s4,-32(s0)
	sd      s5,-24(s0)

/* Load constants used to check the presence of a 0 byte */
	li	s2,0x7f7f7f7f7f7f7f7f
	li	s3,0xffffffffffffffff /* -1 */

/* Check alignment relative to cache line and page */
	li	a2,0x1f   /* 32-1 - maximum size of blocks loaded in .Lkernel */

	and	s5,a0,a2
	and	s6,a1,a2
	mv	a7,zero /* set to zero when ignoring page size */
/* When aligned on 32 bytes, no risk of stepping outside of page */
	bne	s5,s6,.Linit_align_check
	beqz	s5,.Lprologue
.Linit_align_check:
	li	a2,0xfff /* 4096-1 - Page size. */
	li	a7,0xfd0  /* 4048 - Maximum offset*/
.Lalign_check:
	and	s5,a0,a2
	and	s6,a1,a2
	bgt	s5,a7,.Lbyte_by_byte
	bgt	s6,a7,.Lbyte_by_byte
.Lprologue:
/* Entered prologue - we have at least 32 bytes before page end */
/* Load initial values */
	and	s5,a0,a2
	and	s6,a1,a2
	ld	t0,0(a0)
	ld	a3,0(a1)

	ld	t1,8(a0)
	ld	a4,8(a1)

	ld	t2,16(a0)
	ld	a5,16(a1)

	ld	t3,24(a0)
	ld	a6,24(a1)

        addi    a0,a0,32 /* 4*8 */
        addi    a1,a1,32 /* 4*8 */
	bgt	s5,a7,.Lkernel_epilogue
	bgt	s6,a7,.Lkernel_epilogue
.Lkernel:
/* Double 0 */
/* Branch on missmatch */
	bne     t0,a3,.Ldiff0
	and	t4,a3,s2
	or	t5,a3,s2
        ld	t0,0(a0)
        ld      a3,0(a1)

/* Double 1 */
        bne     t1,a4,.Ldiff1
	and	t6,a4,s2
	or	s4,a4,s2
        ld      t1,8(a0)
        ld      a4,8(a1)

	and	s5,a0,a2
	add	t6,t6,s2
	add	t4,t4,s2

	and	s6,a1,a2
	or	t6,t6,s4
	or	t4,t4,t5

/* Check if there are zeros in the first 2 doubles */
	bne	t4,s3,.Lzero_end
	bne	t6,s3,.Lzero_end
/* Double 2 */
        bne     t2,a5,.Ldiff2
	and	t4,a5,s2
	or	t5,a5,s2
        ld      t2,16(a0)
        ld      a5,16(a1)
/* Double 3 */
        bne     t3,a6,.Ldiff3
	and	t6,a6,s2
	or	s4,a6,s2
        ld      t3,24(a0)
        ld      a6,24(a1)

	add	t6,t6,s2
	add	t4,t4,s2
	and	s5,a0,a2

	or	t6,t6,s4
	or	t4,t4,t5
	and	s6,a1,a2

/* Check if there are zeros in the last 2 doubles */
	bne	t4,s3,.Lzero_end
	bne	t6,s3,.Lzero_end
        addi    a0,a0,32 /* 4*8 */
        addi    a1,a1,32 /* 4*8 */
/* if a7==0, addresses are aligned -> keep looping no matter what */
	beqz	a7,.Lkernel
/* otherwise check both addresses are not near page border */
	bgt	s5,a7,.Lkernel_epilogue

	ble	s6,a7,.Lkernel

/* process the already loaded doubles */
.Lkernel_epilogue:
/* Double 0 */
	bne     t0,a3,.Ldiff
       	and	t4,a3,s2
	or	t5,a3,s2

	add	t4,t4,s2
	mv      t0, t1
        mv      a3, a4

/* Double 1 */
        bne     t1,a4,.Ldiff1_ep
	and	t6,a4,s2
	or	s4,a4,s2

	add	t6,t6,s2
        mv      t0, t2
        mv      a3, a5

	or	t6,t6,s4
	or	t4,t4,t5

	bne	t4,s3,.Lzero_end
	bne	t6,s3,.Lzero_end
/* Double 2 */
        bne     t2,a5,.Ldiff
	and	t4,a5,s2
	or	t5,a5,s2

	add	t4,t4,s2
        mv      t0, t3
        mv      a3, a6

/* Double 3 */
        bne     t3,a6,.Ldiff3_ep
	and	t6,a6,s2
	or	s4,a6,s2

	add	t6,t6,s2

	or	t6,t6,s4
	or	t4,t4,t5

	bne	t4,s3,.Lzero_end
	bne	t6,s3,.Lzero_end

/* When current addresses are near page border, compute byte-by-byte*/
.Lbyte_by_byte:
	and	s5,a0,a2
	and	s6,a1,a2
	lbu	t0,0(a0)
	lbu	a3,0(a1)

	bne	t0,a3,.Lend
	beqz	t0,.Lzero_end
	addi 	a0,a0,1
	addi 	a1,a1,1

/* Check alignment */
	ble	s5,a7,.Lprologue
	ble	s6,a7,.Lprologue
	j	.Lbyte_by_byte

/* Jumps cost 4 cycles, falling through all 4 cases is actually faster */
.Ldiff3:
/* When coming from the kernel, need to reload previous values */
	add     t4,t4,s2
        ld	t2,-16(a0)
        ld      a5,-16(a1)
.Ldiff3_ep:
	or      t4,t4,t5
	mv      t0, t2
        mv      a3, a5
        bne     t4,s3,.Ldiff
	mv	t0, t3
	mv	a3, a6
	j	.Ldiff0
.Ldiff2:
	mv	t0, t2
	mv	a3, a5
	j	.Ldiff0
.Ldiff1:
/* When coming from the kernel, need to reload previous values */
	add     t4,t4,s2
        ld	t0,-32(a0)
        ld      a3,-32(a1)
.Ldiff1_ep:
	or      t4,t4,t5
	bne     t4,s3,.Ldiff
	mv	t0, t1
	mv	a3, a4
.Ldiff0:
/* 	We found a diff in 1 double.
   	Find the first byte causing it */
.Ldiff:
/* byte 0 */
	andi	a4,t0,0xff
	andi	a5,a3,0xff
	bne	a4,a5,.Lend0
	beqz	a4,.Lzero_end

/* byte 1 */
	srli	a6,t0,8
	srli	a7,a3,8
	andi	a7,a7,0xff
	andi	a6,a6,0xff
	bne	a6,a7,.Lend1
	beqz	a6,.Lzero_end

/* byte 2 */
	srli	a4,t0,16
	srli	a5,a3,16
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0
	beqz	a4,.Lzero_end

/* byte 3 */
	srli	a6,t0,24
	srli	a7,a3,24
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1
	beqz	a6,.Lzero_end

/* byte 4 */
	srli	a4,t0,32
	srli	a5,a3,32
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0
	beqz	a4,.Lzero_end

/* byte 5 */
	srli	a6,t0,40
	srli	a7,a3,40
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1
	beqz	a6,.Lzero_end

/* byte 6 */
	srli	a4,t0,48
	srli	a5,a3,48
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0
	beqz	a4,.Lzero_end

/* byte 7 */
	srli	a6,t0,56
	srli	a7,a3,56
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1
	beqz	a6,.Lzero_end

.Lend0:
	sub	a0,a4,a5
	j	.Lret
.Lend1:
	sub	a0,a6,a7
	j	.Lret
.Lend:
	sub	a0,t0,a3
	j	.Lret
.Lzero_end:
	mv	a0,zero
.Lret:
	ld      s2,-48(s0)
	ld      s3,-40(s0)
	ld      s4,-32(s0)
	ld      s5,-24(s0)
	ld      s6,-72(s0)
	ld      ra,72(sp)
	ld      s0,64(sp)
	addi    sp,sp,80
	ret
.cfi_endproc
.type name, @function
.size strcmp, . -strcmp
