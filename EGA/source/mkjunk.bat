@echo off
rem MKJUNK
rem Converting result of LINKing junkcode.asm
rem to byte sequence to be added to ES1842 BIOS
rem Usage: mkjunk junkcode.exe junk.bin
rem Where juncode.exe is result of linking,
rem       junk.bin is result of this routine.
rem Uses DEBUG.EXE
rem
rem L.Yadrennikov (RCgoff), Oct-2022
rem
echo n%1 >mkjunk.deb
echo l>>mkjunk.deb
echo rcx>>mkjunk.deb
echo 2f0>>mkjunk.deb
echo n%2>>mkjunk.deb
echo wcs:3960>>mkjunk.deb
echo q>>mkjunk.deb
debug<mkjunk.deb
del mkjunk.deb

