#pragma once

#include "LogCallbacks.h"

namespace Quartz
{
	class Application;

	namespace _Log
	{
		void LogCallback(LogCallbackFunc callback, LogLevel level, const char* message, ...);
	}
}

#define AppLogCallback(callback, level, message, ...) \
	Quartz::_Log::LogCallback(callback, level, message, ##__VA_ARGS__);