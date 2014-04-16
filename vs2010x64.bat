call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x64

set CURRDIR=%CD%

set QTDIR=C:/Qt/Qt5.1.1/5.1.1/msvc2010
set FFMPEGDIR=C:\projects\IIM\IIMSDK\libraries\ffmpeg\bin\x64
set BUILD_DIR=%CURRDIR%\build\VS2010\x64
set ICU_DIR=C:\projects\3rdparty\icu
set CMAKE_PREFIX_PATH=%QTDIR%/lib/cmake

mkdir %BUILD_DIR%
cd %BUILD_DIR%
set path=%QTDIR%/bin;%FFMPEGDIR%;%ICU_DIR%\bin64;%path%

echo %CD%
set cmake_call=cmake -G "Visual Studio 10 Win64" %CURRDIR%
echo %cmake_call%

cmake -G "Visual Studio 10 Win64" %CURRDIR%

set SOLUTION_FILE=%BUILD_DIR%\IIMSDK.sln

devenv "%SOLUTION_FILE%"