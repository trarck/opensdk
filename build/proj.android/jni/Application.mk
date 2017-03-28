APP_STL := gnustl_static

APP_CPPFLAGS := -frtti -std=c++11 -fsigned-char
APP_LDFLAGS := 
APP_ABI :=armeabi armeabi-v7a

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DYH_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
