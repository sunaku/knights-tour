@echo off
set VC_PATH=C:\Program Files\Microsoft Visual Studio
set CG_PATH=C:\Program Files\NVIDIA Corporation\Cg
set GLEW_PATH=C:\Documents and Settings\All Users\glew-1.3.5-win32\glew

rem basename (without .c extension) of C source file
set src=main

rem load the VC++ environment variables
call "%VC_PATH%\VC98\Bin\vcvars32.bat"

rem compile the C program
cl /nologo /TP /MTd /W3 /GX /Od /D WIN32 /D _DEBUG /D _CONSOLE /D _MBCS /GZ /c %src%.c /I "%CG_PATH%\include" /I "%GLEW_PATH%\include" /D GLEW_STATIC

rem link the C program
link /nologo /subsystem:console /debug /machine:I386 /out:%src%.exe %src%.obj /pdbtype:sept kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /libpath:"%CG_PATH%\lib" cg.lib cgGL.lib /libpath:"%GLEW_PATH%\lib" glew32s.lib /NODEFAULTLIB:"libc"

rem run the C program
%src%.exe

rem clean up
erase %src%.exe %src%.ilk %src%.obj %src%.pdb
