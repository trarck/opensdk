rem win
gyp --depth=. -Dlibrary=static_library --generator-output=build/win32

rem ios
gyp --depth=. -Dlibrary=static_library -f xcode -DOS=ios --generator-output=build/ios

rem mac
gyp --depth=. -Dlibrary=static_library -f xcode -DOS=mac --generator-output=build/mac

rem android
gyp --depth=. -Dlibrary=static_library -f android -DOS=android -Gandroid_top_dir=./