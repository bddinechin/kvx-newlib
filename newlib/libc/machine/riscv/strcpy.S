.cfi_sections .debug_frame
.text
.global strcpy
.type   strcpy, @function
/* Parameters:
	$a0: dest pointer
	$a1: src pointer
	This Algorithm performs speculative loads up to
	the end of the page containing the end of each string.
	May cause illegal memory access warnings.
*/
strcpy:
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

	li	s2,0x7f7f7f7f7f7f7f7f
	li	s3,0xffffffffffffffff /* -1 */
	li	a2,0x1f   /* 32-1 - Kernel memory block size. */

	and	s6,a1,a2
	mv	a7,zero /* set to zero when ignoring page size */
	beqz	s6,.Lprologue
.Linit_align_check:
	li	a2,0xfff /* 4096-1 - Page size. */
	li	a7,0xfd0  /* 4048 - Maximum offset*/
.Lalign_check:
	and	s5,a0,a2
	and	s6,a1,a2
	bgt	s6,a7,.Lbyte_by_byte
.Lprologue:
/* Entered prologue - we have at least 32 bytes before page end */
/* Load initial values */
	and	s5,a0,a2
	and	s6,a1,a2
	ld	a3,0(a1)

	ld	a4,8(a1)

	ld	a5,16(a1)

	ld	a6,24(a1)

        addi    a1,a1,32 /* 4*8 */
	mv	t0,a3
	bgtu	s6,a7,.Lkernel_epilogue
.Lkernel:
/* Branch on missmatch */
/* Double 0 */
	and	t4,t0,s2
	or	t5,t0,s2
        ld      a3,0(a1)
	mv	t1,a4

/* Double 1 */
	and	t6,t1,s2
	or	s4,t1,s2
        ld      a4,8(a1)
	mv	t2,a5

	and	s5,a0,a2
	add	t6,t6,s2
	add	t4,t4,s2

	and	s6,a1,a2
	or	t6,t6,s4
	or	t4,t4,t5

	bne	t4,s3,.Lzero
	bne	t6,s3,.Lzero1
////////// HERE WE KNOW THAT NO ZERO WAS FOUND in the first 2
	sd	t0,0(a0)
	mv	t3,a6

/* Double 2 */
	and	t4,t2,s2
	or	t5,t2,s2
        ld      a5,16(a1)
	sd	t1,8(a0)
/* Double 3 */
	and	t6,t3,s2
	or	s4,t3,s2
        ld      a6,24(a1)
	mv	t0,a3

	add	t6,t6,s2
	add	t4,t4,s2
	and	s5,a0,a2

	or	t6,t6,s4
	or	t4,t4,t5
	and	s6,a1,a2

	bne	t4,s3,.Lzero2
	bne	t6,s3,.Lzero3
////////// HERE WE KNOW THAT NO ZERO WAS FOUND in the last 2
        addi    a1,a1,32 /* 4*8 */
	//mv	t3,a6
	sd	t2,16(a0)
        addi    a0,a0,32 /* 4*8 */

	sd	t3,-8(a0) /* 24-32 */
/* if a7==0, addresses are aligned -> keep looping no matter what */
	beqz	a7,.Lkernel
/* otherwise check both addresses are not near page border */

	bleu	s6,a7,.Lkernel

/* process the already loaded doubles */
.Lkernel_epilogue:
/* Double 0 */
       	and	t4,t0,s2
	or	t5,t0,s2

	add	t4,t4,s2
	mv	t1,a4

/* Double 1 */
	and	t6,a4,s2
	or	s4,a4,s2

	add	t6,t6,s2
	mv	t2,a5

	or	t6,t6,s4
	or	t4,t4,t5

	bne	t4,s3,.Lzero
	bne	t6,s3,.Lzero1
	sd	t0,0(a0)
/* Double 2 */
	and	t4,a5,s2
	or	t5,a5,s2
	sd	t1,8(a0)

	add	t4,t4,s2
	mv	t3,a6

/* Double 3 */
	and	t6,a6,s2
	or	s4,a6,s2

	add	t6,t6,s2

	or	t6,t6,s4
	or	t4,t4,t5

	bne	t4,s3,.Lzero2
	bne	t6,s3,.Lzero3
	sd	t2,16(a0)
	sd	t3,24(a0)
        addi    a0,a0,32 /* make a0 catch up with a1 */

/* When current addresses are near page border, compute byte-by-byte*/
.Lbyte_by_byte:
	and	s6,a1,a2
	lbu	t0,0(a1)

	beqz	t0,.Lzero_end
	sb	t0,0(a0)
	addi 	a0,a0,1
	addi 	a1,a1,1

/* Check alignment */
	bleu	s6,a7,.Lprologue
	j	.Lbyte_by_byte

.Lzero3:
	sd	t2,16(a0)
	mv	t0,t3
	addi	a0,a0,24
	j	.Lzero
.Lzero2:
	mv	t0,t2
	addi	a0,a0,16
	j	.Lzero
.Lzero1:
	sd	t0,0(a0)
	addi	a0,a0,8
	mv	t0,t1
/* pre-requisite: suspicious value must be in t0 */
.Lzero:
/* byte 0 */
	andi	a4,t0,0xff
	beqz	a4,.Lzero_end
	sb	a4,0(a0)
	addi 	a0,a0,1

/* byte 1 */
	srli	a6,t0,8
	andi	a6,a6,0xff
	beqz	a6,.Lzero_end
	sb	a6,0(a0)
	addi 	a0,a0,1

/* byte 2 */
	srli	a4,t0,16
	andi	a4,a4,0xff
	beqz	a4,.Lzero_end
	sb	a4,0(a0)
	addi 	a0,a0,1

/* byte 3 */
	srli	a6,t0,24
	andi	a6,a6,0xff
	beqz	a6,.Lzero_end
	sb	a6,0(a0)
	addi 	a0,a0,1

/* byte 4 */
	srli	a4,t0,32
	andi	a4,a4,0xff
	beqz	a4,.Lzero_end
	sb	a4,0(a0)
	addi 	a0,a0,1

/* byte 5 */
	srli	a6,t0,40
	andi	a6,a6,0xff
	beqz	a6,.Lzero_end
	sb	a6,0(a0)
	addi 	a0,a0,1

/* byte 6 */
	srli	a4,t0,48
	andi	a4,a4,0xff
	beqz	a4,.Lzero_end
	sb	a4,0(a0)
	addi 	a0,a0,1

/* byte 7 */
	srli	a6,t0,56
	andi	a6,a6,0xff
	beqz	a6,.Lzero_end
/* UNREACHABLE CODE */
	unimp

/* Found the null character */
.Lzero_end:
	sb	zero,0(a0)
	mv	a0,zero
.Lret:
	ld      a0,-56(s0)
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
.size strcpy, . -strcpy
