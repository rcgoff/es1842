	PAGE,120
	SUBTTL	MONOCHROME CHARACTER GENERATOR	-  ALPHA SUPPLEMENT
CODE	SEGMENT PUBLIC
	PUBLIC	CGMN_FDG
CGMN_FDG	LABEL	BYTE

; STRUCTURE OF THIS FILE
;	DB	XXH	WHERE XX IS THE HEX CODE FOR THE FOLLOWING CHAR
;	DB	[BYTES 0 - 13 OF THAT CHARACTER]
;	...
;	DB	00H	INDICATES NO MORE REPLACEMENTS TO BE DONE

;1842{
IFNDEF RUS_EGA
	DB	01DH					;
	DB	000H,000H,000H,000H,024H,066H,0FFH,066H ; TH_1D
	DB	024H,000H,000H,000H,000H,000H		; BT_1D
	DB	022H					;
	DB	000H,063H,063H,063H,022H,000H,000H,000H ; TH_22 "
	DB	000H,000H,000H,000H,000H,000H		; BT_22 "
	DB	02BH					;
	DB	000H,000H,000H,018H,018H,018H,0FFH,018H ; TH_2B +
	DB	018H,018H,000H,000H,000H,000H		; BT_2B +
	DB	02DH					;
	DB	000H,000H,000H,000H,000H,000H,0FFH,000H ; TH_2D -
	DB	000H,000H,000H,000H,000H,000H		; BT_2D -
	DB	04DH					;
	DB	000H,000H,0C3H,0E7H,0FFH,0DBH,0C3H,0C3H ; TH_4D M
	DB	0C3H,0C3H,0C3H,000H,000H,000H		; BT_4D M
	DB	054H					;
	DB	000H,000H,0FFH,0DBH,099H,018H,018H,018H ; TH_54 T
	DB	018H,018H,03CH,000H,000H,000H		; BT_54 T
	DB	056H					;
	DB	000H,000H,0C3H,0C3H,0C3H,0C3H,0C3H,0C3H ; TH_56 V
	DB	066H,03CH,018H,000H,000H,000H		; BT_56 V
	DB	057H					;
	DB	000H,000H,0C3H,0C3H,0C3H,0C3H,0DBH,0DBH ; TH_57 W
	DB	0FFH,066H,066H,000H,000H,000H		; BT_57 W
	DB	058H					;
	DB	000H,000H,0C3H,0C3H,066H,03CH,018H,03CH ; TH_58 X
	DB	066H,0C3H,0C3H,000H,000H,000H		; BT_58 X
	DB	059H					;
	DB	000H,000H,0C3H,0C3H,0C3H,066H,03CH,018H ; TH_59 Y
	DB	018H,018H,03CH,000H,000H,000H		; BT_59 Y
	DB	05AH					;
	DB	000H,000H,0FFH,0C3H,086H,00CH,018H,030H ; TH_5A Z
	DB	061H,0C3H,0FFH,000H,000H,000H		; BT_5A Z
	DB	06DH					;
	DB	000H,000H,000H,000H,000H,0E6H,0FFH,0DBH ; TH_60 L.C. M
	DB	0DBH,0DBH,0DBH,000H,000H,000H		; BT_60 L.C. M
	DB	076H					;
	DB	000H,000H,000H,000H,000H,0C3H,0C3H,0C3H ; TH_76 L.C. V
	DB	066H,03CH,018H,000H,000H,000H		; BT_76 L.C. V
	DB	077H					;
	DB	000H,000H,000H,000H,000H,0C3H,0C3H,0DBH ; TH_77 L.C. W
	DB	0DBH,0FFH,066H,000H,000H,000H		; BT_77 L.C. W
	DB	091H					;
	DB	000H,000H,000H,000H,06EH,03BH,01BH,07EH ; TH_91
	DB	0D8H,0DCH,077H,000H,000H,000H		; BT_91
	DB	09BH					;
	DB	000H,018H,018H,07EH,0C3H,0C0H,0C0H,0C3H ; TH_9B
	DB	07EH,018H,018H,000H,000H,000H		; BT_98
	DB	09DH					;
	DB	000H,000H,0C3H,066H,03CH,018H,0FFH,018H ; TH_9D
	DB	0FFH,018H,018H,000H,000H,000H		; BT_9D
	DB	09EH					;
	DB	000H,0FCH,066H,066H,07CH,062H,066H,06FH ; TH_9E
	DB	066H,066H,0F3H,000H,000H,000H		; BT_9E
	DB	0F1H					;
	DB	000H,000H,018H,018H,018H,0FFH,018H,018H ; TH_F1
	DB	018H,000H,0FFH,000H,000H,000H		; BT_F1
	DB	0F6H					;
	DB	000H,000H,018H,018H,000H,000H,0FFH,000H ; TH_F6
	DB	000H,018H,018H,000H,000H,000H		; BT_F6
ELSE
	DB	12CH DUP (00H)
ENDIF
;}1842
	DB	000H					; NO MORE
CODE	ENDS
	END
