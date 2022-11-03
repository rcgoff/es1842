;junk code found at the end of ES-1842 EGA BIOS firmware
;formal disassembly by L.Yadrennikov (RCgoff) Oct-2022
include junkseg.inc
include junkdata.inc

caller	MACRO val1, val2, val3, val4, val5
local skip87, maclop, skpinc, exit
	CMP     [val1],00 
	JZ      exit     
	TEST    [data11],01  
	JNZ     skip87
	funcall funct08 val2 val3
	CALL    funct07           
skip87: funcall funct04 10h val4
	CALL    funct03           
	CALL    nearsub2                
	CALL    nearsub3                
	MOV     AX,[val1]           
	MOV     [BP-02],AX          
maclop:	CMP     WORD PTR [BP-02],00
	JZ      exit     
	TEST    [data01],01  
	JZ      skpinc      
	INC     [data02]     
skpinc:	XOR     AX,AX               
	PUSH    AX                  
	PUSH    [BP-02]             
	CALL    nearsub4                
	CALL    nearsub3                
	MOV     SI,[BP-02]          
	MOV     AX,[SI+val5]          
	MOV     [BP-02],AX          
	JMP     maclop      
exit:
	ENDM

funcall MACRO funname, nestval1, nestval2
	MOV     AX,nestval1             
	PUSH    AX                  
	MOV     AX,nestval2             
	PUSH    AX                  
	CALL    funname           
	ENDM

func5call MACRO arg
	MOV     BX,arg  
	PUSH    BX       
	XOR     BX,BX    
	PUSH    BX       
	PUSH    [data05]   
	CALL    funct05
	ENDM

exitproc MACRO
	MOV     SP,BP
	POP     BP
	RET
	ENDM


_TEXT segment word public 'CODE'
assume cs:_TEXT,ds:DATA

org	2b54h
nearsub1 label near
org	30d9h
nearsub2 label near
org	30f4h
nearsub3 label near
org	313dh
nearsub4 label near
org	35f7h
nearsub5 label near

org	3960h	;code beginning
proc1 proc near
	JZ      p1lbl1
	CMP     BYTE PTR [SI+16h],02
	JNZ     p1lbl2
p1lbl1:	MOV     SI,[BP-02]
	PUSH    [SI+0Ah]
	CALL    nearsub5
p1lbl2:	CALL    nearsub3
	MOV     SP,BP
	POP     BP
	RET     0002
proc1 endp

proc2 proc near
	CALL    nearsub6
	ADD     [BX+SI],AL
	MOV     [data24],00
	MOV     [data41],0000
	MOV     AX,[data16]
	MOV     [data15],AL
	MOV     [data36],0000
	MOV     [data35],0000
	MOV     [data31],0000
	MOV     [data03],0Ah
	MOV     [data04],00
	MOV     [data12],00
	MOV     [data07],00
	MOV     [data34],00
	MOV     [data33],00
	MOV     [data25],00
	MOV     [data26],00
	MOV     [data14],01
	MOV     [data08],01
	MOV     [data05],0000
	MOV     [data06],0000
	MOV     [data13],01
	MOV     [data20],0001
	MOV     [data21],0000
	MOV     [data18],0039h
	MOV     [data17],003Ah
	MOV     [data32],0FFFFh
	MOV     [data19],0050h
	MOV     [data40],0000
label3:	MOV     SI,[data40]
	SHL     SI,1
	MOV     WORD PTR [SI+028Ch],0000
	MOV     AX,[data40]
	INC     AX
	MOV     [data40],AX
	CMP     AX,0004
	JNZ     label3
	MOV     [data27],0000
	MOV     [data28],0000
	MOV     [data29],0000
	MOV     [data38],0000
	MOV     [data42],0000
	MOV     AX,[data10]
	XOR     AH,AH
	SUB     AX,0009
	NEG     AX
	MOV     [data09],AX
	XOR     AX,AX
	PUSH    AX
	PUSH    AX
	CALL    nearsub1
	exitproc
proc2 endp

proc3 proc near
	CALL    nearsub6
	ADD     AL,[BX+SI]
	caller data28 0dh 0ca0h 1c31h 1eh
	caller data29 0fh 0caeh 1c43h 1ch
	CMP     [data05],00
	JZ      exit               
	TEST    [data01],01 
	JZ      skip_inc               
	INC     [data02]
skip_inc:
	TEST    [data11],01
	JNZ     skip_fun8
	funcall funct08 13h 0cbeh
skip_fun8:
	func5call 1c54h
	CALL    funct07
	funcall funct04 13h 0cbeh
	func5call 1c58h
	CALL    funct03
	CALL    nearsub2     
exit:	exitproc
proc3 endp

proc4 proc near
	CALL    nearsub6                
	ADD     [BX+SI],AL          
	CMP     [data41],00 
	JNZ     p4skip                
	CMP     [data30],00  
	JZ      p4lbl2                
	MOV     [data40],0001
p4loop:	MOV     SI,[data40]           
	MOV     AX,[SI+02ADh]        
	MOV     [SI+0118h],AL        
	MOV     AX,[data40]           
	INC     AX                  
	MOV     [data40],AX           
	CMP     AX,0020h             
	JNZ     p4loop                
	MOV     [data23],0Dh  
	MOV     [data37],0001
	CALL    funct10           
	MOV     [data37],0001
	CMP     [data44],00  
	JZ      p4lbl2                
	CALL    nearsub9
p4lbl2:	CMP     [data41],00 
	JNZ     p4skip                
	MOV     [data44],01  
	MOV     [data45],41h  
	MOV     AX,offset data41             
	PUSH    AX                  
	CALL    nearsub8                
p4skip:	MOV     [data43],01  
	PUSH    [data41]              
	CALL    nearsub7                
	MOV     AX,offset data41             
	PUSH    AX                  
	CALL    funct02
	exitproc
proc4 endp

proc5 proc near
	CALL    nearsub6
	ADD     AL,00
	CALL    funct06
	MOV     AX,000Ah
	PUSH    AX
	funcall funct01 09h 0d32h
	CALL    funct11
	CALL    proc2
	CALL    proc3
	PUSH    [data39]
	CALL    funct09
	TEST    [data22],01
	JZ      p5lbl1
	MOV     AX,[data46]
	DB      0Fh
	PUSH    SS
	ADD     DX,[DI-56h]
org	3ca8h
p5lbl1 label near
proc5 endp

org	3ecbh
nearsub6 label near
org	7ff1h
nearsub7 label near
org 	8a9eh
nearsub8 label near
org	0ff1dh
nearsub9 label near

_TEXT ends
	end proc3
