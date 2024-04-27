#pragma once

namespace Quartz
{
#ifndef QUARTZ_LOG_LEVEL_ENUM

	enum LogLevel
	{
		LOG_LEVEL_TRACE,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_FATAL,
		LOG_LEVEL_SUCCESS,
		LOG_LEVEL_FAIL
	};

#define QUARTZ_LOG_LEVEL_ENUM

#endif

	typedef void(*LogCallbackFunc)(LogLevel level, const char* message);

	namespace _Log
	{
		extern LogCallbackFunc smLogCallback;
	}
}