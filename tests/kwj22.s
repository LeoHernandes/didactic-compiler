	.file	"kwj22.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, -8(%rbp)
	movl	$0, -12(%rbp)
	movl	$2, -16(%rbp)
	movl	$3, -20(%rbp)
	movl	-8(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.L2
	movl	-16(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jge	.L2
	movl	$2, -4(%rbp)
	jmp	.L3
.L2:
	movl	$3, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
