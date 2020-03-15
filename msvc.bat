@echo off
if "%1"==""  echo --init-cl , --compile  !!  
if "%1"=="--init-cl" "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
if "%1"=="--compile" cl /std:c++14 /EHsc /D_ITERATOR_DEBUG_LEVEL=0 /O2 "colormap.cpp" ^
/I "C:\Program Files (x86)\IntelSWTools\openvino\opencv\include" ^
/link ^
/libpath:"C:\Program Files (x86)\IntelSWTools\openvino\opencv\lib" "opencv_highgui411.lib" "opencv_video411.lib" "opencv_videoio411.lib" "opencv_core411.lib"  "opencv_imgproc411.lib" "opencv_dnn411.lib" ^
/DLL ^
/OUT:"colormap.dll"