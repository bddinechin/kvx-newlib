.cfi_sections .debug_frame
.text
.global memcpy
.type	memcpy, @function
/* Parameters:
   $a0: dest pointer
   $a1: src pointer
   $a2: length in bytes
*/
memcpy:
	.cfi_startproc
	addi	sp,sp,-80
	sd	ra,72(sp)
	sd	s0,64(sp)
	addi	s0,sp,80
	sd	a0,-56(s0)
	sd	a1,-64(s0)
	sd	a2,-72(s0)
	beqz	a2,.Lret

.Lprologue:
	li	t6,32 /* 4*8 */
	mv	a3,a0

	bltu	a2,t6,.Lenter_long_by_long

	ld	t0,0(a1)

	ld	t1,8(a1)

	ld	t2,16(a1)

	sub	a2,a2,t6
	ld	t3,24(a1)
	addi	a1,a1,32
	mv	a4,a1
	mv	a5,a3

	bltu	a2,t6,.Lkernel_epilogue
.Lkernel:
	sub	a2,a2,t6
	sd	t0,0(a5)
	ld	t0,0(a4)

	mv	a6,a4
	mv	a7,a5
	sd	t1,8(a5)
	ld	t1,8(a4)

	addi	a3,a3,32
	addi	a1,a1,32
	sd	t2,16(a5)
	ld	t2,16(a4)

	mv	a4,a1
	mv	a5,a3
	sd	t3,24(a7)
	ld	t3,24(a6)
	bgeu	a2,t6,.Lkernel

.Lkernel_epilogue:
	sd	t0,0(a3)

	sd	t1,8(a3)

	sd	t2,16(a3)

	sd	t3,24(a3)

	addi	a3,a3,32
	beqz	a2,.Lret

.Lenter_long_by_long:
	li	t6,8
	bltu	a2,t6,.Lenter_byte_by_byte
	ld	t1,0(a1)
	addi	a1,a1,8
	sub	a2,a2,t6
	bltu	a2,t6,.Llong_by_long_epilogue
.Llong_by_long:
	ld	t0,0(a1)
	sd	t1,0(a3)
	mv	t1,t0
	addi	a1,a1,8
	sub	a2,a2,t6
	addi	a3,a3,8
	bgeu	a2,t6,.Llong_by_long
.Llong_by_long_epilogue:
	sd	t1,0(a3)
	addi	a3,a3,8
.Lenter_byte_by_byte:
	li	t6,1
	bleu	a2,zero,.Lret
	lbu	t1,0(a1)
	addi	a1,a1,1
	sub	a2,a2,t6
	bltu	a2,t6,.Lbyte_by_byte_epilogue
.Lbyte_by_byte:
	lbu	t0,0(a1)
	sb	t1,0(a3)
	mv	t1,t0
	addi	a1,a1,1
	sub	a2,a2,t6
	addi	a3,a3,1
	bgeu	a2,t6,.Lbyte_by_byte
.Lbyte_by_byte_epilogue:
	sb	t1,0(a3)
.Lret:
	ld	ra,72(sp)
	ld	s0,64(sp)
	addi	sp,sp,80
	ret
.cfi_endproc
.size	memcpy, . -memcpy
