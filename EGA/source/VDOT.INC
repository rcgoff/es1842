	SUBTTL	VDOT.INC
	PAGE
;------------------------------------------------
; ENTRY						:
;	DX = ROW				:
;	CX = COLUMN				:
;	BH = PAGE				:
; EXIT						:
;	BX = OFFSET INTO REGEN			:
;	AL = BIT MASK FOR COLUMN BYTE		:
;------------------------------------------------
DOT_SUP_1	PROC	NEAR

;----- OFFSET = PAGE OFFSET + ROW * BYTES/ROW + COLUMN/8

	MUL	WORD PTR CRT_COLS		; ROW * BYTES/ROW
	PUSH	CX				; SAVE COLUMN VALUE
	SHR	CX,1				; DIVIDE BY EIGHT TO
	SHR	CX,1				;  DETERMINE THE BYTE THAT
	SHR	CX,1				;  THIS DOT IS IN
						;  (8 BITS/BYTE)
	ADD	AX,CX				; BYTE OFFSET INTO PAGE
	MOV	BL,BH				; GET PAGE INTO BL
	SUB	BH,BH				; ZERO
	MOV	CX,BX				; COUNT VALUE
	MOV	BX,CRT_LEN			; LENGTH OF ONE PAGE
	JCXZ	DS_2				; PAGE ZERO
DS_3:
	ADD	AX,BX				; BUMP TO NEXT PAGE
	LOOP	DS_3				; DO FOR THE REST
DS_2:
	POP	CX				; RECOVER COLUMN VALUE
	MOV	BX,AX				; REGEN OFFSET
	AND	CL,07H				; SHIFT COUNT FOR BIT MASK
	MOV	AL,080H				; MASK BIT
	SHR	AL,CL				; POSITION MASK BIT
	RET
DOT_SUP_1	ENDP

;--------------------------------------------------------
; THIS SUBROUTINE DETERMINES THE REGEN BYTE LOCATION	:
; OF THE INDICATED ROW COLUMN VALUE IN GRAPHICS MODE.	:
; ENTRY --						:
;  DX = ROW VALUE (0-199)				:
;  CX = COLUMN VALUE (0-639)				:
; EXIT --						:
;  SI = OFFSET INTO REGEN BUFFER FOR BYTE OF INTEREST	:
;  AH = MASK TO STRIP OFF THE BITS OF INTEREST		:
;  CL = BITS TO SHIFT TO RIGHT JUSTIFY THE MASK IN AH	:
;  DH = # BITS IN RESULT				:
;--------------------------------------------------------
R3	PROC	NEAR
	PUSH	BX			; SAVE BX DURING OPERATION
	PUSH	AX			; WILL SAVE AL DURING OPERATION

;----- DETERMINE 1ST BYTE IN IDICATED ROW BY MULTIPLYING ROW VALUE BY 40
;----- ( LOW BIT OF ROW DETERMINES EVFN/ODD, 80 BYTES/ROW

	MOV	AL,40
	PUSH	DX			; SAVE ROW VALUE
	AND	DL,0FEH			; STRIP OFF ODD/EVEN BIT
	MUL	DL			; AX HAS ADDRESS OF 1ST BYTE
					;  OF INDICATED ROW
	POP	DX			; RECOVER IT
	TEST	DL,1			; TEST FOR EVEN/ODD
	JZ	R4			; JUMP IF EVEN ROW
	ADD	AX,2000H		; OFFSET TO LOCATION OF ODD ROWS
R4:					; EVEN_ROW
	MOV	SI,AX			; MOVE POINTER TO SI
	POP	AX			; RECOVER AL VALUE
	MOV	DX,CX			; COLUMN VALUE TO DX

;----- DETERMINE GRAPHICS MODE CURRENTLY IN EFFECT

;----------------------------------------------------------------
; SET UP THE REGISTERS ACCORDING TO THE MODE			:
;  CH = MASK FOR LOW OF COLUMN ADDRESS ( 7/3 FOR HIGH/MED RES)	:
;  CL = # OF ADDRESS BITS IN COLUMN VALUE ( 3/2 FOR H/M)	:
;  BL = MASK TO SELECT BITS FROM POINTED BYTE (80H/C0H FOR H/M) :
;  BH = NUMBER OF VALID BITS IN POINTED BYTE ( 1/2 FOR H/M)	:
;----------------------------------------------------------------

	MOV	BX,2C0H
	MOV	CX,302H			; SET PARMS FOR MED RES
	CMP	CRT_MODE,6
	JC	R5			; HANDLE IF MED ARES
	MOV	BX,180H
	MOV	CX,703H			; SET PARMS FOR HIGH RES

;----- DETERMINE BIT OFFSET IN BYTE FROM COLUMN MASK

R5:
	AND	CH,DL			; ADDRESS OF PEL WITHIN BYTE TO CH

;----- DETERMINE BYTE OFFSET FOR THIS LOCATION IN COLUMN

	SHR	DX,CL			; SHIFT BY CORRECT AMOUNT
	ADD	SI,DX			; INCREMENT THE POINTER
	MOV	DH,BH			; GET THE # OF BITS IN RESULT TO DH

;----- MULTIPLY BH (VALID BITS IN BYTE) BY CH (BIT OFFSET)

	SUB	CL,CL			; ZERO INTO STORAGE LOCATION
R6:
	ROR	AL,1			; LEFT JUSTIFY THE VALUE
					;  IN AL (FOR WRITE)
	ADD	CL,CH			; ADD IN THE BIT OFFSET VALUE
	DEC	BH			; LOOP CONTROL
	JNZ	R6			; ON EXIT, CL HAS SHIFT COUNT
					;  TO RESTORE BITS
	MOV	AH,BL			; GET MASK TO AH
	SHR	AH,CL			; MOVE THE MASK TO CORRECT LOCATION
	POP	BX			; RECOVER REG
	RET				; RETURN WITH EVERYTHING SET UP
R3	ENDP

;----------------------------------------------------------------
; READ DOT -- WRITE DOT						:
;	THESE ROUTINES WILL WRITE A DOT, OR READ THE DOT AT	:
;	THE INDICATED LOCATION					:
; ENTRY --							:
;   DX = ROW (0-199)	(THE ACTUAL VALUE DEPENDS ON THE MODE)	:
;   CX = COLUMN ( 0-639) ( THE VALUES ARE NOT RANGE CHECKED )	:
;   AL = DOT VALUE TO WRITE (1,2 OR 4 BITS DEPENDING ON MODE,	:
;	REQ'D FOR WRITE DOT ONLY, RIGHT JUSTIFIED)              :
;	BIT 7 OF AL=1 INDICATES XOR THE VALUE INTO THE LOCATION :
;   DS = DATA SEGMENT						:
;   ES = REGEN SEGMENT						:
;								:
; EXIT								:
;	AL = DOT VALUE READ, RIGHT JUSTIFIED, READ ONLY		:
;----------------------------------------------------------------

;----- WRITE DOT

AHC:
	ASSUME	DS:ABS0
	CMP	CRT_MODE,7
	JA	WRITE_DOT_2

WRITE_DOT	PROC	NEAR
	ASSUME	DS:ABS0,ES:NOTHING
	PUSH	DX
	SRLOAD	ES,0B800H
	POP	DX
	PUSH	AX			; SAVE DOT VALUE
	PUSH	AX			;  TWICE
	CALL	R3			; DETERMINE BYTE POSITION OF THE DOT
	SHR	AL,CL			; SHIFT TO SET UP THE BITS FOR OUTPUT
	AND	AL,AH			; STRIP OFF THE OTHER BITS
	MOV	CL,ES:[SI]		; GET THE CURRENT BYTE
	POP	BX			; RECOVER XOR FLAG
	TEST	BL,80H			;  IS IT ON
	JNZ	R2			; YES, XOR THE DOT
	NOT	AH			; SET THE MASK TO REMOVE THE
	AND	CL,AH			;  INDICATED BITS
	OR	AL,CL			; OR IN THE NEW VALUE OF THOSE BITS
R1:					; FINISH_DOT
	MOV	ES:[SI],AL		; RESTORE THE BYTE IN MEMORY
	POP	AX
	JMP	V_RET
R2:					; XOR_DOT
	XOR	AL,CL			; EXCLUSIVE OR THE DOTS
	JMP	R1			; FINISH UP THE WRITING
WRITE_DOT	ENDP

WRITE_DOT_2	PROC	NEAR
	CMP	CRT_MODE,0FH
	JB	NO_ADJ2
	CALL	MEM_DET				; BASE CARD
	JC	NO_ADJ2
	AND	AL,10000101B			; 85H, XOR C2 C0 MASK
	MOV	AH,AL
	SHL	AH,1				; EXPAND C0 TO C1, C2 TO C3
	OR	AL,AH				; BUILD ?(80H) + (0,3,C,F)
NO_ADJ2:
	PUSH	AX
	MOV	AX,DX				; ROW VALUE
	CALL	DOT_SUP_1			; BX=OFFSET, AL=BIT MASK
	MOV	DH,3
	MOV	DL,GRAPH_ADDR			; GRAPHICS CHIP
	MOV	AH,G_BIT_MASK			; BIT MASK REGISTER
	CALL	OUT_DX				; SET BIT MASK
	PUSH	DX
	SRLOAD	ES,0A000H			; REGEN SEGMENT
	POP	DX
	POP	AX				; RECOVER COLOR
	MOV	CH,AL				; SAVE COLOR
	TEST	CH,080H				; SEE IF XOR
	JZ	WD_A				; NO XOR
	MOV	AH,G_DATA_ROT			; DO XOR
	MOV	AL,018H				; XOR FUNCTION
	CALL	OUT_DX				; SET THE REGISTER
	JMP	WD_B				; SKIP THE BLANK
WD_A:						; BLANK THE DOT
	MOV	DL,SEQ_ADDR			; SEQUENCER
	MOV	AH,S_MAP			; MAP MASK
	MOV	AL,0FFH				; ENABLE ALL MAPS
	CALL	OUT_DX				; SET THE REGISTER
	MOV	AL,ES:[BX]			; LATCH DATA
	SUB	AL,AL				; ZERO
	MOV	ES:[BX],AL			; BLANK THE DOT
WD_B:						; SET THE COLOR MAP MASK
	MOV	DL,SEQ_ADDR			; SEQUENCER
	MOV	AH,S_MAP			; MAP MASK REGISTER
	MOV	AL,CH				; COLOR VALUE
	AND	AL,0FH				; VALUES 0-15
	CALL	OUT_DX				; SET IT
	MOV	AL,ES:[BX]			; LATCH DATA
	MOV	AL,0FFH				; WRITE VALUE
	MOV	ES:[BX],AL			; SET THE DOT

;----- NORMALIZE THE ENVIRONMENT

	CALL	OUT_DX				; ALL MAPS ON
	MOV	DL,GRAPH_ADDR			; GRAPHICS CHIPS
	MOV	AH,G_DATA_ROT			; XOR REGISTER
	SUB	AL,AL				; NORMAL WRITES
	CALL	OUT_DX				; SET IT
	MOV	AH,G_BIT_MASK			; BIT MASK
	MOV	AL,0FFH				; ALL BITS ON
	CALL	OUT_DX				; SET IT
	JMP	V_RET				; WRITE DOT DONE
WRITE_DOT_2	ENDP

RD_S	PROC	NEAR
	ASSUME	DS:ABS0
	PUSH	AX
	PUSH	DX
	SRLOAD	ES,0A000H
	POP	DX
	POP	AX
	MOV	AX,DX
	CALL	DOT_SUP_1
	MOV	CH,7
	SUB	CH,CL
	SUB	DX,DX
	MOV	AL,0
	RET
RD_S	ENDP

RD_1S	PROC	NEAR
	MOV	CL,CH
	MOV	AH,4
	PUSH	DX
	MOV	DH,3
	MOV	DL,GRAPH_ADDR
	CALL	OUT_DX
	POP	DX
	MOV	AH,ES:[BX]
	SHR	AH,CL
	AND	AH,1
	RET
RD_1S	ENDP

;----- READ DOT

AHD:
	ASSUME	DS:ABS0
	CMP	CRT_MODE,7
	JA	R_1

READ_DOT	PROC	NEAR
	ASSUME	DS:ABS0,ES:NOTHING
	PUSH	DX
	SRLOAD	ES,0B800H
	POP	DX
	CALL	R3			; DETERMINE BYTE POSITION OF DOT
	MOV	AL,ES:[SI]		; GET THE BYTE
	AND	AL,AH			; MASK OFF THE OTHER BITS IN THE BYTE
	SHL	AL,CL			; LEFT JUSTIFY THE VALUE
	MOV	CL,DH			; GET NUMBER OF BITS IN RESULT
	ROL	AL,CL			; RIGHT JUSTIFY THE RESULT
	JMP	V_RET
READ_DOT	ENDP

R_1:
	CMP	CRT_MODE,0FH
	JB	READ_DOT_2
	CALL	MEM_DET
	JC	READ_DOT_2

READ_DOT_1	PROC	NEAR			; 2 MAPS
	ASSUME	DS:ABS0, ES:NOTHING
	CALL	RD_S
	CALL	RD_1S
	OR	DL,AH
	SHL	AH,1
	OR	DL,AH
	MOV	AL,2
	CALL	RD_1S
	SHL	AH,1
	SHL	AH,1
	OR	DL,AH
	SHL	AH,1
	OR	DL,AH
	MOV	AL,DL
	JMP	V_RET
READ_DOT_1	ENDP

READ_DOT_2	PROC	NEAR			; 4 MAPS
	ASSUME	DS:ABS0, ES:NOTHING
	CALL	RD_S
RD_2A:
	CALL	RD_1S
	MOV	CL,AL
	SHL	AH,CL
	OR	DL,AH
	INC	AL
	CMP	AL,3
	JBE	RD_2A
	MOV	AL,DL
	JMP	V_RET
READ_DOT_2	ENDP

;------------------------------------------------------------------------
; WRITE_TTY	WRITE TELETYPE TO ACTIVE PAGE				:
;	THIS INTERFACE PROVIDES A TELETYPE LIKE INTERFACE TO THE VIDEO	:
;	CARD.	THE INPUT CHARACTER  IS WRITTEN TO THE	CURRENT CURSOR	:
;	POSITION, AND THE CURSOR IS MOVED TO THE NEXT POSITION. IF THE	:
;	CURSOR LEAVES THE LAST COLUMN OF THE FIELD,  THE COLUMN IS SET	:
;	TO ZERO,  AND THE ROW VALUE IS INCREMENTED.   IF THE ROW VALUE	:
;	LEAVES THE FIELD,  THE CURSOR IS PLACED ON THE LAST ROW, FIRST	:
;	COLUMN,	 AND THE ENTIRE SCREEN IS SCROLLED UP ONE LINE.	  WHEN	:
;	THE SCREEN IS SCROLLED UP, THE ATTRIBUTE FOR FILLING THE NEWLY	:
;	BLANKED LINE IS READ FROM THE  CURSOR POSITION ON THE PREVIOUS	:
;	LINE BEFORE THE SCROLL,	 IN CHARACTER MODE,  IN GRAPHICS MODE,	:
;	THE 0 COLOR IS USED.						:
; ENTRY									:
;	(AH) = CURRENT CRT MODE						:
;	(AL) = CHARACTER TO BE WRITTEN					:
;	 NOTE THAT BACK SPACE, CAR RET, BELL AND LINE FEED ARE HANDLED	:
;	 AS COMMANDS RATHER THAN AS DISPLAYABLE GRAPHICS		:
;	(BL) = FOREGROUND COLOR FOR CHAR WRITE IF CURRENTLY IN A	:
;	 GRAPHICS MODE							:
; EXIT									:
;	ALL REGISTERS SAVED						:
;------------------------------------------------------------------------
AHE:
	ASSUME	CS:CODE,DS:ABS0
	PUSH	AX				; SAVE REGISTERS
	MOV	BH,ACTIVE_PAGE			; GET THE ACTIVE PAGE
	PUSH	BX				; SAVE
	MOV	BL,BH				; GET PAGE TO BL
	XOR	BH,BH				; CLEAR HIGH BYTE
	SAL	BX,1				; *2 FOR WORD OFFSET
	MOV	DX,[BX + OFFSET CURSOR_POSN]	; CURSOR, ACTIVE PAGE
	POP	BX				; RECOVER

;----- DX NOW HAS THE CURRENT CURSOR POSITION

	CMP	AL,0DH				; IS IT CARRIAGE RETURN
	JE	U9				; CAR_RET
	CMP	AL,0AH				; IS IT A LINE FEED
	JE	U10				; LINE_FEED
	CMP	AL,08H				; IS IT A BACKSPACE
	JE	U8				; BACK_SPACE
	CMP	AL,07H				; IS IT A BELL
	JE	U11				; BELL

;----- WRITE THE CHAR TO THE SCREEN

	MOV	AH,10				; WRITE CHAR ONLY
	MOV	CX,1				; ONLY ONE CHAR
	INT	10H				; WRITE THE CHAR

;----- POSITION THE CURSOR FOR NEXT CHAR

	INC	DL
	CMP	DL,BYTE PTR CRT_COLS		; TEST FOR COLUMN OVERFLOW
	JNZ	U7				; SET_CURSOR
	SUB	DL,DL				; COLUMN FOR CURSOR
	CMP	DH,ROWS
	JNZ	U6				; SET_CURSOR_INC

;----- SCROLL REQUIRED

U1:
	CALL	SET_CPOS			; SET THE CURSOR

;----- DETERMINE VALUE TO FILL WITH DURING SCROLL

	MOV	AL,CRT_MODE			; GET THE CURRENT MODE
	CMP	AL,4
	JB	U2				; READ-CURSOR
	SUB	BH,BH				; FILL WITH BACKGROUND
	CMP	AL,7
	JNE	U3				; SCROLL-UP
U2:						; READ-CURSOR
	MOV	AH,8
	INT	10H				; READ CHAR/ATTR
	MOV	BH,AH				; STORE IN BH
U3:						; SCROLL-UP
	MOV	AX,601H				; SCROLL ONE LINE
	SUB	CX,CX				; UPPER LEFT CORNER
	MOV	DH,ROWS				; LOWER RIGHT ROW
	MOV	DL,BYTE PTR CRT_COLS		; LOWER RIGHT COLUMN
	DEC	DL
U4:						; VIDEO-CALL-RETURN
	INT	10H				; SCROLL UP THE SCREEN
U5:						; TTY-RETURN
	POP	AX				; RESTORE THE CHARACTER
	JMP	V_RET				; RETURN TO CALLER
U6:						; SET-CURSOR-INC
	INC	DH				; NEXT ROW
U7:						; SET-CURSOR
	MOV	AH,2
	JMP	U4				; ESTABLISH THE NEW CURSOR

;----- BACK SPACE FOUND

U8:
	OR	DL,DL				; ALREADY AT END OF LINE
	JZ	U7				; SET CURSOR
	DEC	DL				; NO -- JUST MOVE IT BACK
	JMP	U7				; SET_CURSOR

;----- CARRIAGE RETURN FOUND

U9:
	SUB	DL,DL				; MOVE TO FIRST COLUMN
	JMP	U7				; SET_CURSOR

;----- LINE FEED FOUND

U10:
	CMP	DH,ROWS				; BOTTOM OF SCREEN
	JNE	U6				; YES, SCROLL THE SCREEN
	JMP	U1				; NO, JUST SET THE CURSOR

;----- BELL FOUND

U11:
	MOV	BL,2				; SET UP COUNT FOR BEEP
	CALL	BEEP				; SOUND THE POD BELL
	JMP	U5				; TTY_RETURN


;----- CURRENT VIDEO STATE

AHF:
	ASSUME	DS:ABS0
	MOV	AH,BYTE PTR CRT_COLS		; GET NUMBER OF COLUMNS
	MOV	BH,ACTIVE_PAGE
	MOV	AL,INFO
	AND	AL,080H
	OR	AL,CRT_MODE
	POP	DI
	POP	SI
	POP	CX				; DISCARD BX
	POP	CX
	POP	DX
	POP	DS
	POP	ES
	POP	BP
	IRET

	SUBTTL

