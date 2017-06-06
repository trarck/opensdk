#include "Log.h"
#include "YHStdC.h"
#include "PlatformDefine.h"

NS_OPENSDK_BEGIN

#define MAX_LOG_LENGTH         kMaxLogLen

ScriptDeubgFuncPtr ScriptDebug = nullptr;

void SetScriptDeubgFuncPtr(ScriptDeubgFuncPtr fp)
{
	ScriptDebug = fp;
}

static void _log(const char *format, va_list args)
{
	int bufferSize = MAX_LOG_LENGTH;
	char* buf = nullptr;

	do
	{
		buf = new char[bufferSize];
		if (buf == nullptr)
			return; // not enough memory

		int ret = vsnprintf(buf, bufferSize - 3, format, args);
		if (ret < 0)
		{
			bufferSize *= 2;

			delete[] buf;
		}
		else
			break;

	} while (true);

#if defined(OPENSDK_USE_SCRIPT_LOG)
	if (ScriptDebug != nullptr) {
		ScriptDebug(buf);
	}
#endif

	strcat(buf, "\n");

#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "yh:", "%s", buf);

#elif OPENSDK_TARGET_PLATFORM ==  OPENSDK_PLATFORM_WIN32 || OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WINRT

	int pos = 0;
	int len = strlen(buf);
	char tempBuf[MAX_LOG_LENGTH + 1] = { 0 };
	WCHAR wszBuf[MAX_LOG_LENGTH + 1] = { 0 };

	do
	{
		std::copy(buf + pos, buf + pos + MAX_LOG_LENGTH, tempBuf);

		tempBuf[MAX_LOG_LENGTH] = 0;

		MultiByteToWideChar(CP_UTF8, 0, tempBuf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, tempBuf, sizeof(tempBuf), nullptr, FALSE);
		printf("%s", tempBuf);

		pos += MAX_LOG_LENGTH;

	} while (pos < len);
	fflush(stdout);
#else
	// Linux, Mac, iOS, etc
	fprintf(stdout, "%s", buf);
	fflush(stdout);
#endif
	delete[] buf;
}

void log(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	_log(format, args);
	va_end(args);
}





NS_OPENSDK_END
