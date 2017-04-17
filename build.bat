rem win
gyp --depth=. --Dopensdk_library=static_library --generator-output=build/win32

rem ios
gyp --depth=. --Dopensdk_library=static_library -f xcode -DOS=ios --generator-output=build/ios

rem mac
gyp --depth=. --Dopensdk_library=static_library -f xcode -DOS=mac --generator-output=build/mac

rem android
gyp --depth=. --Dopensdk_library=static_library -f android -DOS=android -Gandroid_top_dir=./

rem make-android
gyp --depth=. --Dopensdk_library=static_library -Dtarget_arch=arm -DOS=android -f make-android  -Gandroid_top_dir=./ --generator-output=build/android

./gyp_opensdk.py -Dtarget_arch=arm -DOS=android -f make-android --generator-output=build/android