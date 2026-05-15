
main.bin:     file format elf32-avr


Disassembly of section .text:

00000080 <wait_500>:
  80:	cf 93       	push	r28
  82:	df 93       	push	r29
  84:	00 d0       	rcall	.+0      	; 0x86 <L0^A>

00000086 <L0^A>:
  86:	00 d0       	rcall	.+0      	; 0x88 <L0^A>

00000088 <L0^A>:
  88:	cd b7       	in	r28, 0x3d	; 61
  8a:	de b7       	in	r29, 0x3e	; 62

0000008c <.LBB2>:
  8c:	1a 82       	std	Y+2, r1	; 0x02
  8e:	19 82       	std	Y+1, r1	; 0x01

00000090 <.Loc.2>:
  90:	13 c0       	rjmp	.+38     	; 0xb8 <.L2>

00000092 <.L5>:
  92:	1c 82       	std	Y+4, r1	; 0x04
  94:	1b 82       	std	Y+3, r1	; 0x03

00000096 <.Loc.4>:
  96:	05 c0       	rjmp	.+10     	; 0xa2 <.L3>

00000098 <.L4>:
  98:	8b 81       	ldd	r24, Y+3	; 0x03
  9a:	9c 81       	ldd	r25, Y+4	; 0x04
  9c:	01 96       	adiw	r24, 0x01	; 1
  9e:	9c 83       	std	Y+4, r25	; 0x04
  a0:	8b 83       	std	Y+3, r24	; 0x03

000000a2 <.L3>:
  a2:	8b 81       	ldd	r24, Y+3	; 0x03
  a4:	9c 81       	ldd	r25, Y+4	; 0x04
  a6:	84 32       	cpi	r24, 0x24	; 36
  a8:	24 ef       	ldi	r18, 0xF4	; 244
  aa:	92 07       	cpc	r25, r18
  ac:	a8 f3       	brcs	.-22     	; 0x98 <.L4>

000000ae <.LBE3>:
  ae:	89 81       	ldd	r24, Y+1	; 0x01
  b0:	9a 81       	ldd	r25, Y+2	; 0x02
  b2:	01 96       	adiw	r24, 0x01	; 1
  b4:	9a 83       	std	Y+2, r25	; 0x02
  b6:	89 83       	std	Y+1, r24	; 0x01

000000b8 <.L2>:
  b8:	89 81       	ldd	r24, Y+1	; 0x01
  ba:	9a 81       	ldd	r25, Y+2	; 0x02
  bc:	87 30       	cpi	r24, 0x07	; 7
  be:	91 05       	cpc	r25, r1
  c0:	40 f3       	brcs	.-48     	; 0x92 <.L5>

000000c2 <.LBE2>:
	...

000000c4 <.Loc.10>:
  c4:	0f 90       	pop	r0
  c6:	0f 90       	pop	r0
  c8:	0f 90       	pop	r0
  ca:	0f 90       	pop	r0
  cc:	df 91       	pop	r29
  ce:	cf 91       	pop	r28
  d0:	08 95       	ret
