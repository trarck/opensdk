
if "%1" NEQ "" set NDK_ROOT=%1
if "%NDK_ROOT%" == "" goto NO_NDK_ROOT 

%NDK_ROOT%\ndk-build -C ./ -j8 NDK_DEBUG=0
goto END

:NO_NDK_ROOT
echo not NDK_ROOT defined.

:END