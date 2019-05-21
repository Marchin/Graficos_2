@echo off
SET "LIB="

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64)) 

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64)) 

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" x641))

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" x64)) 

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" x64)) 

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 13.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" x64)) 

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\vcvarsall.bat" x64)) 


set CompilerFlags=-MDd -nologo -Gm- -GR- -EHsc -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DSLOW=1 -DOPENGL=1 -FC -Zi -fp:fast  
set LinkerFlags= -incremental:no -opt:ref  user32.lib Gdi32.lib winmm.lib shell32.lib 
IF NOT EXIST ..\build mkdir ..\build
IF NOT EXIST ..\build\assimp-vc140-mt.dll (
	copy ..\libs\assimp-vc140-mt.dll ..\build\
)
pushd ..\build
cl %CompilerFlags% -I "..\libs" -DENGINE_EXPORTS=1  ..\engine\engine.cpp -LD -link %LinkerFlags% ..\libs\glfw3.lib ..\libs\assimp-vc140-mt.lib opengl32.lib  -incremental:no  
cl %CompilerFlags% -I "..\libs" -I "..\engine" ..\test_game\main.cpp -link %LinkerFlags% engine.lib
popd