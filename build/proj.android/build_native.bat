if "%NDK_ROOT%" == "" goto NO_NDK_ROOT 

%NDK_ROOT%\ndk-build -C ./ -j8 NDK_MODULE_PATH=../../src/android NDK_TOOLCHAIN_VERSION=4.8 NDK_DEBUG=0
goto END

:NO_NDK_ROOT
echo not NDK_ROOT defined.

:END