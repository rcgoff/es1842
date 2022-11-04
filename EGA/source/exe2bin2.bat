@echo off
rem EXE2BIN2
rem Converts result of EXE2BIN to
rem the file can be burned in ROM.
rem Written for EGA BIOS diassembly.
rem Uses DEBUG.EXE.
rem Usage: exe2bin2 filename size-in-hex checksum
rem        or
rem        exe2bin2 filename size-in-hex ES1842
rem Where: filename - result of exe2bin withoun extension
rem           extension supposed to be E2B
rem        size-in-hex - size of rom in hex bytes,
rem           e.g. 4000 for 16384 bytes
rem        checksum - last byte in ROM in hex
rem        ES1842 - special mode for ES1842 EGA BIOS
rem Example: exe2bin2 vbios40 4000 65
rem 
rem L.Yadrennikov (RCgoff), Nov-2022
rem
echo f cs:100 l %2 00>debug2bin
echo n%1.e2b>>debug2bin
echo l>>debug2bin

if %3 == ES1842 goto es1842_process

rem Add checksum byte
echo ecs:40ff>>debug2bin
echo %3>>debug2bin
goto final

:es1842_process
rem In ES-1842 BIOS some code at the end represent beginning of code
rem Copying this code fragment from begin to the end
echo m cs:100 4af cs:3d50>>debug2bin

rem Only one byte in repeated beginning is different:
rem SAVE_TBL from VPOST.INC have segment address C800 instead of C000.
echo ecs:3e5f>>debug2bin
echo c8>>debug2bin

rem There's mistake in 8x8 font symbol 04h
rem Let's make this mistake
echo ecs:3287>>debug2bin
echo 08>>debug2bin


:final
echo rcx>>debug2bin
echo %2>>debug2bin
echo n%1.bin>>debug2bin
echo wcs:100>>debug2bin
echo q>>debug2bin
debug<debug2bin
del debug2bin
