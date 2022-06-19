#pragma once

#include "Types/String.h"
#include "Window.h"
#include "Surface.h"
#include "DLL.h"

namespace Quartz
{
	typedef void(*WindowResizedCallbackFunc)(Window* pWindow, uSize width, uSize height);
	typedef void(*WindowMovedCallbackFunc)(Window* pWindow, uSize x, uSize y);
	typedef bool(*WindowCloseRequestedCallbackFunc)(Window* pWindow);
	typedef void(*WindowClosedCallbackFunc)(Window* pWindow);
	typedef void(*WindowMaximizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowMinimizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowFocusedCallbackFunc)(Window* pWindow, bool lost);

	enum WindowAPI
	{
		WINDOW_API_NONE,
		WINDOW_API_GLFW,
		WINDOW_API_WINAPI
	};

	struct ApplicationInfo
	{
		WindowAPI	windowApi;
		String		appName;
		String		version;
	};

	class QUARTZAPP_API Application
	{
	protected:
		String		mAppName;
		String		mVersion;
		WindowAPI	mWindowAPI;

		WindowResizedCallbackFunc			mWindowResizedFunc;
		WindowMovedCallbackFunc				mWindowMovedFunc;
		WindowCloseRequestedCallbackFunc	mWindowCloseRequestedFunc;
		WindowClosedCallbackFunc			mWindowClosedFunc;
		WindowMaximizedCallbackFunc			mWindowMaximizedFunc;
		WindowMinimizedCallbackFunc			mWindowMinimizedFunc;
		WindowFocusedCallbackFunc			mWindowFocusedFunc;

	public:
		Application(const ApplicationInfo& appInfo);

		virtual bool Create() = 0;
		virtual void Destroy() = 0;

		virtual Window* CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo) = 0;
		virtual void CloseWindow(Window* pWindow) = 0;
		virtual void DestroyWindow(Window* pWindow) = 0;

		void SetWindowResizedCallback(WindowResizedCallbackFunc callback);
		void SetWindowMovedCallback(WindowMovedCallbackFunc callback);
		void SetWindowCloseRequestedCallback(WindowCloseRequestedCallbackFunc callback);
		void SetWindowClosedCallback(WindowClosedCallbackFunc callback);
		void SetWindowMaximizedCallback(WindowMaximizedCallbackFunc callback);
		void SetWindowMinimizedCallback(WindowMinimizedCallbackFunc callback);
		void SetWindowFocusedCallback(WindowFocusedCallbackFunc callback);

		virtual void Update() = 0;

		virtual void* GetNativeHandle() = 0;
	};

	QUARTZAPP_API Application* CreateApplication(const ApplicationInfo& appInfo);
	QUARTZAPP_API void DestroyApplication(Application* pApp);
}