.syntax unified

.data
input_arr: .int 1, 2, 1, 0
midValue: .int 0
epsilon: .int 2
output_arr: .int 0, 0, 3

.text

@ r0 = intput_arr
@ r1 = midValue
@ r2 = epsilon
@ r3 = output_arr

.global rangeNormalizer

rangeNormalizer:
	push {r4-r12, lr}
	mov fp, sp
	
	@ldr r0, =input_arr
	@mov r1, #0
	@mov r2, #2
	@ldr r3, =output_arr
	
	mov r4, #0  @ r4 = output_idx = 0
	mov r5, #0  @ r5 = input_idx = 0
	
loop:
	ldr r6, [r0, r5, lsl #2] @ r6 = input_arr[r5]
	cmp r6, 0
	beq vege  @ input_arr[r5] == 0
	
	subs r7, r1, r6
	submi r7, r6, r1  @ r7 = |midValue - r6|
	
	add r5, r5, #1
	
	cmp r7, r2
	blt put  @ if |midValue - r6| < epsilon
	b loop
	
put:
	sub r6, r6, r1  @ r6 = r6 - midValue
	str r6, [r3, r4, lsl #2]  @ output_arr[r4] = r6 - midValue
	add r4, r4, #1
	b loop
	
vege:
	mov r0, r4
	
	mov sp, fp
	pop {r4-r12, pc}
