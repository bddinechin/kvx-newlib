.cfi_sections .debug_frame
.text
.global memcmp
.type   memcmp, @function
/* Parameters:
   $a0: a pointer
   $a1: b pointer
   $a2: length in bytes
*/

memcmp:
	.cfi_startproc
/* Save registers */
	addi    sp,sp,-80
	sd      ra,72(sp)
	sd      s0,64(sp)
	addi    s0,sp,80
	sd      a0,-56(s0)
	sd      a1,-64(s0)
	sd      a2,-72(s0)
	sd      s2,-48(s0)
	sd      s3,-40(s0)
	beqz	a2,.Lzero_end

	li	a7,32 /* 4*8 */
.Lprologue:
/* if n<4*8, handle doubles individually */
	bltu	a2,a7,.Lenter_long_by_long
	mv	t5,a0
	mv	t6,a1

/* Load initial values */
        sub     a2,a2,a7
	ld	t0,0(a0)
	ld	a3,0(a1)

        addi    a0,a0,32 /* 4*8 */
        addi    a1,a1,32 /* 4*8 */
	ld	t1,8(t5)
	ld	a4,8(t6)

        mv      s2,t5
        mv      s3,t6
	ld	t2,16(t5)
	ld	a5,16(t6)

	mv	t5,a0
	mv	t6,a1
	ld	t3,24(s2)
	ld	a6,24(s3)
	bltu	a2,a7,.Lkernel_epilogue

.Lkernel:
/* Branch on missmatch */
	bne     t0,a3,.Ldiff0
        sub     a2,a2,a7
        ld      t0,0(t5)
        ld      a3,0(t6)

        bne     t1,a4,.Ldiff1
/* Save the pointer addresses */
        mv      s2,t5
        mv      s3,t6
        ld      t1,8(t5)
        ld      a4,8(t6)

        bne     t2,a5,.Ldiff2
        addi    a0,a0,32 /* 4*8 */
        addi    a1,a1,32 /* 4*8 */
        ld      t2,16(t5)
        ld      a5,16(t6)

        bne     t3,a6,.Ldiff3
/* Update the pointer addresses */
	mv	t5,a0
	mv	t6,a1
        ld      t3,24(s2)
        ld      a6,24(s3)
/* If n>4*8, keep looping */
	bgeu	a2,a7,.Lkernel

/* Otherwise, process the already loaded doubles */
.Lkernel_epilogue:
	bne     t0,a3,.Ldiff0
        mv      t5, t1
        mv      t6, a4

        bne     t1,a4,.Ldiff
        mv      t5, t2
        mv      t6, a5

        bne     t2,a5,.Ldiff
        mv      t5, t3
        mv      t6, a6

        bne     t3,a6,.Ldiff
/* Compute the remaining doubles 1 by 1 */
.Lenter_long_by_long:
	li	a7,8
	bltu	a2,a7,.Lenter_byte_by_byte
.Llong_by_long:
	ld	t5,0(a0)
	ld	t6,0(a1)
	bne	t5,t6,.Ldiff
	addi 	a0,a0,8
	addi 	a1,a1,8
	sub	a2,a2,a7
	bgeu	a2,a7,.Llong_by_long
.Lenter_byte_by_byte:
	li	a7,1
	bleu	a2,zero,.Lzero_end
.Lbyte_by_byte:
	lbu	t0,0(a0)
	lbu	a3,0(a1)
	bne	t0,a3,.Lend
	addi 	a0,a0,1
	addi 	a1,a1,1
	sub	a2,a2,a7
	bgtu	a2,zero,.Lbyte_by_byte
	j	.Lzero_end

/* Jumps cost 4 cycles, falling through all 4 cases is actually faster */
.Ldiff3:
	mv	t2, t3
	mv	a5, a6
.Ldiff2:
	mv	t1, t2
	mv	a4, a5
.Ldiff1:
	mv	t0, t1
	mv	a3, a4
.Ldiff0:
	mv	t5, t0
	mv	t6, a3
.Ldiff:
/* 	We found a diff in 1 double.
   	Find the first byte causing it */
/* byte 0 */
	andi	a4,t5,0xff
	andi	a5,t6,0xff
	bne	a4,a5,.Lend0

/* byte 1 */
	srli	a6,t5,8
	srli	a7,t6,8
	andi	a7,a7,0xff
	andi	a6,a6,0xff
	bne	a6,a7,.Lend1

/* byte 2 */
	srli	a4,t5,16
	srli	a5,t6,16
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0

/* byte 3 */
	srli	a6,t5,24
	srli	a7,t6,24
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1

/* byte 4 */
	srli	a4,t5,32
	srli	a5,t6,32
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0

/* byte 5 */
	srli	a6,t5,40
	srli	a7,t6,40
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1

/* byte 6 */
	srli	a4,t5,48
	srli	a5,t6,48
	andi	a4,a4,0xff
	andi	a5,a5,0xff
	bne	a4,a5,.Lend0

/* byte 7 */
	srli	a6,t5,56
	srli	a7,t6,56
	andi	a6,a6,0xff
	andi	a7,a7,0xff
	bne	a6,a7,.Lend1

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
	mv	a0,a2
.Lret:
	ld      s2,-48(s0)
	ld      s3,-40(s0)
	ld      ra,72(sp)
	ld      s0,64(sp)
	addi    sp,sp,80
	ret
.cfi_endproc
.type name, @function
.size memcmp, . -memcmp
