@echo off

:: Cleanup
rmdir /s /q bin
rmdir /s /q dist

:: Cleanup
mkdir bin
mkdir dist


SET GBDK_HOME=C:/gbdk

SET LCC_COMPILE_BASE=%GBDK_HOME%\bin\lcc -Iheaders -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
SET LCC_COMPILE=%LCC_COMPILE_BASE% -c -o 

:: Required to concatenate the "COMPILE_OBJECT_FILES" via a for loop
SETLOCAL ENABLEDELAYEDEXPANSION

SET "COMPILE_OBJECT_FILES="

:: loop for all files in the default source folder
FOR /R "source/default/" %%X IN (*.c) DO (
    echo Compiling %%~nX ...
    %LCC_COMPILE% bin/%%~nX.o %%X
    SET "COMPILE_OBJECT_FILES=bin/%%~nX.o !COMPILE_OBJECT_FILES!"

)

:: Compile a .gb file from the compiled .o files
%LCC_COMPILE_BASE% -Wm-yC -Wl-yt3 -o dist/FlappyBird.gb !COMPILE_OBJECT_FILES!

endlocal

:: Use the romusage.exe to show the size of the ROM and what makes it up
"lib/romusage" dist\FlappyBird.noi -a