;**********************************************
; Things
;**********************************************
PORTF 	EQU 0x400043FC
HIGH 	EQU 0x0004
LOW		EQU	0x0000

	export pushBuffer
		
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
    align
	
	
pushBuffer PROC
	PUSH {R5-R10}
	
	
	MOV 	R1, #LOW
	MOV		R2, #HIGH
	
	MOV32	R4, PORTF
	MOV 	R3, #2
	MOV		R5, #0

	; Save a copy of color array address
	MOV		R8, R0

PIXEL_LOOP
;MOV		R0, R8		; Restore Color array addr
	MOV		R6, #3
	
COLOR_LOOP
	
	LDRB	R9, [R0]
	MOV		R7, #8
	
BIT_LOOP
	
	MOV		R5, #0
	AND		R10, R9, #0x0080
	CMP		R10, #0
	MOVGT	R5, #1
	
	B		PUSH_BIT
BIT_DONE

	LSL		R9, R9, #1
	SUBS	R7, R7, #1
	BGT		BIT_LOOP
	
	SUBS	R6, R6, #1
	ADD		R0, R0, #1
	BGT		COLOR_LOOP
	
	SUBS	R3, R3, #1
	BGT		PIXEL_LOOP
	B		DONE
	
	
PUSH_BIT
	STR		R2, [R4]
	
	; 18 NOPs plus CMP/STREQ = 20 instruction (0.25us)
	
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	CMP		R5, #0		; Drop if zero else stay high
	STREQ	R1, [R4]
	
	; 27 NOPs plus STR = 28 insr (0.35 us)
	
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	STR		R1, [R4]	; Always drop here
	
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	B		BIT_DONE
	
DONE
	POP		{R5-R10}

	BX		LR
	
	ENDP
    align
    END 
