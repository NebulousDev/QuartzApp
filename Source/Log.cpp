#include "Log.h"

#include "Application.h"

#include <cstdarg>

namespace Quartz
{
	void _Log::LogCallback(LogCallbackFunc callback, LogLevel level, const char* message, ...)
	{
		if (callback)
		{
			va_list args;

			va_start(args, message);
			int bufferSize = vsnprintf(nullptr, 0, message, args);

			char* pBuffer = new char[bufferSize + 1]{};

			va_start(args, message);
			vsprintf(pBuffer, message, args);

			callback(level, pBuffer);

			va_end(args);

			delete[] pBuffer;
		}
	}
}