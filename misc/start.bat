@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\VC\Auxiliary\Build\vcvarsall.bat" x64
set path="W:\Graphics Programming\Misc";%path%
pushd ..\test_game\
4ed main.cpp
popd