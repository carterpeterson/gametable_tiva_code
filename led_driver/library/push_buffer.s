;**********************************************
; Things
;**********************************************
PORT 	EQU 0x400053FC
HIGH 	EQU 0x00FF
LOW		EQU	0x0000

	export push_buffer
		
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
	
	
push_buffer PROC
	PUSH {R5-R10}
	
	MOV		R2, #HIGH
	MOV 	R3, #LOW
	
	MOV32	R4, PORT
	MOV 	R5, #24
	MUL		R1, R1, R5
	MOV		R5, #0

BIT_LOOP
	LDRB	R5, [R0]
	B		PUSH_BIT
BIT_DONE
	ADD		R0, R0, #1
	SUBS	R1, R1, #1
	BGT		BIT_LOOP
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
	NOP
	STR		R5, [R4]
	
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
	STR		R3, [R4]	; Always drop here
	
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
