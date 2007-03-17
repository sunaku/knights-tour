@echo off

rem basename (without .c extension) of C source file
set src=main

rem load the VC++ environment variables
call "C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"

rem compile the C program
cl /nologo /TP /MTd /W3 /GX /Od /I "C:\Program Files\NVIDIA Corporation\Cg\include" /I "C:\Documents and Settings\All Users\glew\include" /D GLEW_STATIC /D WIN32 /D _DEBUG /D _CONSOLE /D _MBCS /GZ /c %src%.c


rem link the C program
link /nologo /subsystem:console /debug /machine:I386 /out:%src%.exe %src%.obj /pdbtype:sept kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /libpath:"C:\Program Files\NVIDIA Corporation\Cg\lib" cg.lib cgGL.lib /libpath:"C:\Documents and Settings\All Users\glew\lib" glew32s.lib /NODEFAULTLIB:"libc"

rem run the C program
%src%.exe

rem clean up
erase %src%.exe %src%.ilk %src%.obj %src%.pdb
