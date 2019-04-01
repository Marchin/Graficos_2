@echo off
set CompilerFlags=-MDd -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DSLOW=1 -DOPENGL=1 -FC -Zi -fp:fast  
set LinkerFlags= -incremental:no -opt:ref  user32.lib Gdi32.lib winmm.lib shell32.lib 
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %CompilerFlags% -I "D:\General Programming\Graphics Programming\libraries" -DENGINE_EXPORTS=1  ..\engine\engine.cpp -LD -link %LinkerFlags% ..\libraries\glfw3.lib opengl32.lib  -incremental:no  
cl %CompilerFlags% -I "D:\General Programming\Graphics Programming\libraries" -I "D:\General Programming\Graphics Programming\engine" ..\test_game\main.cpp -link %LinkerFlags% engine.lib
popd