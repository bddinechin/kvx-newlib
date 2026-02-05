.cfi_sections .debug_frame
.text
.global memset
.type   memset, @function
/* Parameters:
   $a0: s pointer
   $a1: c constant
   $a2: length in bytes
*/
memset:
	.cfi_startproc
/* Save registers */
	addi    sp,sp,-80
	sd      ra,72(sp)
	sd      s0,64(sp)
	addi    s0,sp,80
	sd      a0,-56(s0)
	sd      a1,-64(s0)
	sd      a2,-72(s0)
	beqz	a2,.Lret

	li	a7,1

/* Check alignment on double */
	andi	t0,a0,0x7
	beqz	t0,.Lprologue
	
.Lalign:
	sb	a1,0(a0)
	addi 	a0,a0,1
	sub	a2,a2,a7
	andi	t0,a0,0x7
	beqz	t0,.Lprologue
	bleu	a2,zero,.Lret
	j	.Lalign
	
.Lprologue:
	li	a7,32 /* 4*8 */
/* Distribute constant in double */
  	slli a3, a1, 8
  	or  a1, a1, a3
  	slli a3, a1, 16
  	or  a1, a1, a3
	slli a3, a1, 32
  	or  a1, a1, a3

/* if n<4*8, handle doubles individually */
	bltu	a2,a7,.Lenter_long_by_long
.Lkernel:
        sub     a2,a2,a7
        sd      a1,0(a0)

        sd      a1,8(a0)

        sd      a1,16(a0)

        sd      a1,24(a0)
        addi    a0,a0,32 /* 4*8 */
/* If n>4*8, keep looping */
	bgeu	a2,a7,.Lkernel

/* Compute the remaining doubles 1 by 1 */
.Lenter_long_by_long:
	li	a7,8
	bltu	a2,a7,.Lenter_byte_by_byte
.Llong_by_long:
	sub	a2,a2,a7
	sd	a1,0(a0)
	addi 	a0,a0,8
	bgeu	a2,a7,.Llong_by_long
.Lenter_byte_by_byte:
	li	a7,1
	bleu	a2,zero,.Lret
.Lbyte_by_byte:
	sb	a1,0(a0)
	addi 	a0,a0,1
	sub	a2,a2,a7
	bgtu	a2,zero,.Lbyte_by_byte
.Lret:
	ld      a0,-56(s0)
	ld      ra,72(sp)
	ld      s0,64(sp)
	addi    sp,sp,80
	ret
.cfi_endproc
.type name, @function
.size memset, . -memset
