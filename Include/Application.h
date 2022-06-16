#pragma once

#include "Types/String.h"
#include "Window.h"
#include "DLL.h"

namespace Quartz
{
	typedef void(*WindowResizedCallbackFunc)(Window* pWindow, uSize width, uSize height);
	typedef void(*WindowMovedCallbackFunc)(Window* pWindow, uSize x, uSize y);
	typedef void(*WindowClosedCallbackFunc)(Window* pWindow);
	typedef void(*WindowMaximizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowMinimizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowFocusedCallbackFunc)(Window* pWindow, bool lost);

	struct ApplicationInfo
	{
		String appName;
		String version;
	};

	class Application
	{
	protected:
		String mAppName;
		String mVersion;

		WindowResizedCallbackFunc		mWindowResizedFunc;
		WindowMovedCallbackFunc			mWindowMovedFunc;
		WindowClosedCallbackFunc		mWindowClosedFunc;
		WindowMaximizedCallbackFunc		mWindowMaximizedFunc;
		WindowMinimizedCallbackFunc		mWindowMinimizedFunc;
		WindowFocusedCallbackFunc		mWindowFocusedFunc;

	public:
		Application(const ApplicationInfo& appInfo);

		virtual bool Create() = 0;
		virtual void Destroy() = 0;

		virtual Window* CreateWindow(const WindowInfo& info) = 0;
		virtual void DestroyWindow(Window* pWindow) = 0;

		QUARTZ_API void SetWindowResizedCallback(WindowResizedCallbackFunc callback);
		QUARTZ_API void SetWindowMovedCallback(WindowMovedCallbackFunc callback);
		QUARTZ_API void SetWindowClosedCallback(WindowClosedCallbackFunc callback);
		QUARTZ_API void SetWindowMaximizedCallback(WindowMaximizedCallbackFunc callback);
		QUARTZ_API void SetWindowMinimizedCallback(WindowMinimizedCallbackFunc callback);
		QUARTZ_API void SetWindowFocusedCallback(WindowFocusedCallbackFunc callback);

		virtual void Update() = 0;

		virtual void* GetNativeHandle() = 0;
	};

	QUARTZ_API Application* CreateApplication(const ApplicationInfo& appInfo);
	QUARTZ_API void DestroyApplication(Application* pApp);
}