#makefile for IBM & soviet EGA BIOS disassembly
#Written for MAKE.EXE from Borland Turbo Assembler 5.0
#Bulid was succesfully tested on Win7 32-bit with:
#MAKE:
#	-MAKE from Borland TASM 5.0
#       -NMAKE from Build Tools for Visual studio 2019
#Assemblers:
#	-TASM32 from Borland TASM 5.0
#       -MASM 5.0
#Linkers:
#       -LINK from MASM 3.0, MASM 5.0
#       -LINK16 from MASM32 kit (http://masm32.com)
#Misc:
#       -DEBUG (probably any version, tested with Win7)
#       -EXE2BIN (probably any version, tested with Win7)
#L.Yadrennikov (RCgoff), Nov-2022

assembler=TASM32
#assembler=MASM

!if "$(assembler)" == "TASM32"
asm_parm= /m0 /l
!endif

#linker=LINK
linker=LINK16


all: ega-ibm.bin ega-1842.bin ega-rob.bin ega-4041.bin

# Original IBM EGA version

vbiosibm.obj: vbios.asm vfront.inc vpost.inc vparms.inc v1-5.inc vscroll.inc vgrw.inc vdot.inc vchgen.inc vprsc.inc
	$(assembler) $(asm_parm) VBIOS.ASM, VBIOSIBM.OBJ, VBIOSIBM.LST;

fnt_IBM\vcgmn.obj: fnt_IBM\vcgmn.asm
	$(assembler) $(asm_parm) fnt_IBM\VCGMN.ASM, fnt_IBM\VCGMN.OBJ, fnt_IBM\VCGMN.LST;

fnt_IBM\vcgddot.obj: fnt_IBM\vcgddot.asm
	$(assembler) $(asm_parm) fnt_IBM\VCGDDOT.ASM, fnt_IBM\VCGDDOT.OBJ, fnt_IBM\VCGDDOT.LST;

vcgmnfdg.obj: vcgmnfdg.asm
	$(assembler) $(asm_parm) VCGMNFDG.ASM, VCGMNFDG.OBJ, VCGMNFDG.LST;

vend.obj: vend.asm
	$(assembler) $(asm_parm) VEND.ASM, VEND.OBJ, VEND.LST;

ega-ibm.bin: vbiosibm.obj fnt_IBM\vcgmn.obj vcgmnfdg.obj fnt_IBM\vcgddot.obj vend.obj exe2bin2.bat
	$(linker) VBIOSIBM fnt_IBM\VCGMN VCGMNFDG fnt_IBM\VCGDDOT VEND;
	EXE2BIN VBIOSIBM.EXE VBIOSIBM.E2B
	exe2bin2 vbiosibm 4000 46
	del VBIOSIBM.E2B
	del VBIOSIBM.EXE
	del EGA-IBM.BIN
	ren VBIOSIBM.BIN EGA-IBM.BIN


# ES-1842 version

vbios42.obj: vbios.asm vfront.inc vpost.inc vparms.inc v1-5.inc vscroll.inc vgrw.inc vdot.inc vchgen.inc vprsc.inc
	$(assembler) /DRUS_EGA $(asm_parm) VBIOS.ASM, VBIOS42.OBJ, VBIOS42.LST;

fnt_1842\vcgmn.obj: fnt_1842\vcgmn.asm
	$(assembler) $(asm_parm) fnt_1842\VCGMN.ASM, fnt_1842\VCGMN.OBJ, fnt_1842\VCGMN.LST;

fnt_1842\vcgddot.obj: fnt_1842\vcgddot.asm
	$(assembler) $(asm_parm) fnt_1842\VCGDDOT.ASM, fnt_1842\VCGDDOT.OBJ, fnt_1842\VCGDDOT.LST;

vcgmnfdr.obj: vcgmnfdg.asm
	$(assembler) /DRUS_EGA $(asm_parm) VCGMNFDG.ASM, VCGMNFDR.OBJ, VCGMNFDR.LST;

junkcode.obj: junkcode.asm junkdata.inc junkseg.inc
	$(assembler) $(asm_parm) JUNKCODE.ASM, JUNKCODE.OBJ, JUNKCODE.LST;

ega-1842.bin: vbios42.obj fnt_1842\vcgmn.obj vcgmnfdr.obj fnt_1842\vcgddot.obj vend.obj junkcode.obj mkjunk.bat exe2bin2.bat
	$(linker) VBIOS42 fnt_1842\VCGMN VCGMNFDR fnt_1842\VCGDDOT VEND;
	EXE2BIN VBIOS42.EXE VBIOS42.E2B
	$(linker) JUNKCODE;
	mkjunk junkcode.exe junk.bin
	copy /b VBIOS42.E2B + junk.bin VBIOS42.E2B
	exe2bin2 vbios42 4000 ES1842
	del VBIOS42.E2B
	del VBIOS42.EXE
	del JUNKCODE.EXE
	del JUNK.BIN
	del EGA-1842.BIN
	ren VBIOS42.BIN EGA-1842.BIN


# 006ROB 007ROB version

vbiosrob.obj: vbios.asm vfront.inc vpost.inc vparms.inc v1-5.inc vscroll.inc vgrw.inc vdot.inc vchgen.inc vprsc.inc
	$(assembler) /DRUS_EGA /DROB $(asm_parm) VBIOS.ASM, VBIOSROB.OBJ, VBIOSROB.LST;

fnt_ROB\vcgmn.obj: fnt_ROB\vcgmn.asm
	$(assembler) $(asm_parm) fnt_ROB\VCGMN.ASM, fnt_ROB\VCGMN.OBJ, fnt_ROB\VCGMN.LST;

fnt_ROB\vcgddot.obj: fnt_ROB\vcgddot.asm
	$(assembler) $(asm_parm) fnt_ROB\VCGDDOT.ASM, fnt_ROB\VCGDDOT.OBJ, fnt_ROB\VCGDDOT.LST;


ega-rob.bin: vbiosrob.obj fnt_ROB\vcgmn.obj vcgmnfdr.obj fnt_ROB\vcgddot.obj vend.obj exe2bin2.bat
	$(linker) VBIOSROB fnt_ROB\VCGMN VCGMNFDR fnt_ROB\VCGDDOT VEND;
	EXE2BIN VBIOSROB.EXE VBIOSROB.E2B
	exe2bin2 vbiosrob 4000 30
	del VBIOSROB.E2B
	del VBIOSROB.EXE
	del EGA-ROB.BIN
	ren VBIOSROB.BIN EGA-ROB.BIN


# 0040 0041 version (ES-1849)

fnt_4041\vcgmn.obj: fnt_4041\vcgmn.asm
	$(assembler) $(asm_parm) fnt_4041\VCGMN.ASM, fnt_4041\VCGMN.OBJ, fnt_4041\VCGMN.LST;

fnt_4041\vcgddot.obj: fnt_4041\vcgddot.asm
	$(assembler) $(asm_parm) fnt_4041\VCGDDOT.ASM, fnt_4041\VCGDDOT.OBJ, fnt_4041\VCGDDOT.LST;

ega-4041.bin: vbiosrob.obj fnt_4041\vcgmn.obj vcgmnfdr.obj fnt_4041\vcgddot.obj vend.obj exe2bin2.bat
	$(linker) VBIOSROB fnt_4041\VCGMN VCGMNFDR fnt_4041\VCGDDOT VEND;
	EXE2BIN VBIOSROB.EXE VBIOS41.E2B
	exe2bin2 VBIOS41 4000 65
	del VBIOS41.E2B
	del VBIOSROB.EXE
	del EGA-4041.BIN
	ren VBIOS41.BIN EGA-4041.BIN


clean:
	for /r %f in (*.obj) do del %f
	for /r %f in (*.LST) do del %f
	for /r %f in (*.bin) do del %f
