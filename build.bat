REM @ECHO OFF
IF NOT WHERE /q clang-cl (
	ECHO "*** LLVM not detected ***"
	ECHO "Please install LLVM and try again."
	ECHO "http://llvm.org/releases/download.html"
	GOTO:eof
)
IF NOT WHERE /q cmake (
	ECHO "*** Cmake not detected ***"
	ECHO "Please install Cmake and try again."
	ECHO "https://cmake.org/download/#latest"
	GOTO:eof
)
IF NOT WHERE /q msbuild (
	ECHO "*** MSBuild not detected ***"
	ECHO "Please install Visual Studio and try again."
	GOTO:eof
)
IF DEFINED "%1" (
	REM ECHO  "Using %1 as a configuration"
	SET configuration=%1
) ELSE (
	REM ECHO "Setting configuration Release"
	SET configuration=Release
)
IF DEFINED "%2" (
	REM ECHO  "Using %2 as a platform"
	SET platform=%2
) ELSE (
	IF EXIST "%ProgramFiles(x86)%" (
		REM ECHO "Detected 64-bit platform"
		SET platform=x64
	) ELSE (
		REM ECHO "Detected 32-bit platform"
		SET platform=x86
	)
)
ECHO "Building %configuration% version for %platform%"
IF NOT EXIST ./build MKDIR build
PUSHD build
IF "%platform%" == "x64" SET VS_FULL=Visual Studio 14 2015 Win64
IF "%platform%" == "x64" SET VS_PLATFORM=x64
IF "%platform%" == "x86" SET VS_FULL=Visual Studio 14 2015
IF "%platform%" == "x86" SET VS_PLATFORM=Win32
cmake -T "LLVM-vs2014" -G "%VS_FULL%" --config "%configuration%" ..
msbuild Boids.Experiment.sln /p:Configuration="%configuration%" /p:Platform="%VS_PLATFORM%" /m
POPD
