#pragma once

#include "Types/String.h"
#include "Window.h"
#include "Surface.h"
#include "DLL.h"
#include "LogCallbacks.h"

namespace Quartz
{
	typedef void(*WindowResizedCallbackFunc)(Window* pWindow, uSize width, uSize height);
	typedef void(*WindowMovedCallbackFunc)(Window* pWindow, uSize x, uSize y);
	typedef bool(*WindowCloseRequestedCallbackFunc)(Window* pWindow);
	typedef void(*WindowClosedCallbackFunc)(Window* pWindow);
	typedef void(*WindowMaximizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowMinimizedCallbackFunc)(Window* pWindow, bool restored);
	typedef void(*WindowFocusedCallbackFunc)(Window* pWindow, bool lost);

	typedef void(*KeyCallbackFunc)(Window* pWindow, uInt16 scancode, bool down, bool repeat);
	typedef void(*KeyTypedCallbackFunc)(Window* pWindow, char character, bool repeat);
	typedef void(*MouseMovedCallbackFunc)(Window* pWindow, uSize mouseX, uSize mouseY);
	typedef void(*MouseMovedRelativeCallbackFunc)(Window* pWindow, sSize relX, sSize relY);
	typedef void(*MouseEnteredCallbackFunc)(Window* pWindow, bool entered);


	enum WindowAPI
	{
		WINDOW_API_NONE,
		WINDOW_API_GLFW,
		WINDOW_API_WINAPI
	};

	enum ApplicationHint
	{
		APPLICATION_HINT_USE_RAW_INPUT = 0x1
	};

	typedef flags32 ApplicationHints;

	struct ApplicationInfo
	{
		WindowAPI			windowApi;
		String				appName;
		String				version;
		LogCallbackFunc		logCallback;
		ApplicationHints	hints;
	};

	class QUARTZAPP_API Application
	{
	protected:
		String				mAppName;
		String				mAppVersion;
		WindowAPI			mWindowAPI;
		LogCallbackFunc		mLogCallback;
		ApplicationHints	mHints;

		WindowResizedCallbackFunc			mWindowResizedFunc;
		WindowMovedCallbackFunc				mWindowMovedFunc;
		WindowCloseRequestedCallbackFunc	mWindowCloseRequestedFunc;
		WindowClosedCallbackFunc			mWindowClosedFunc;
		WindowMaximizedCallbackFunc			mWindowMaximizedFunc;
		WindowMinimizedCallbackFunc			mWindowMinimizedFunc;
		WindowFocusedCallbackFunc			mWindowFocusedFunc;
		KeyCallbackFunc						mKeyFunc;
		KeyTypedCallbackFunc				mKeyTypedFunc;
		MouseMovedCallbackFunc				mMouseMovedFunc;
		MouseMovedRelativeCallbackFunc		mMouseMovedRelativeFunc;
		MouseEnteredCallbackFunc			mMouseEnteredFunc;

	public:
		Application(const ApplicationInfo& appInfo);

		virtual Window* CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo) = 0;
		virtual void CloseWindow(Window* pWindow) = 0;
		virtual void DestroyWindow(Window* pWindow) = 0;

		virtual bool IsRawInputAvailable() const = 0;
		virtual bool IsRawInputEnabled() const = 0;
		virtual void UseRawInput(bool useRawInput) = 0;

		void SetWindowResizedCallback(WindowResizedCallbackFunc callback);
		void SetWindowMovedCallback(WindowMovedCallbackFunc callback);
		void SetWindowCloseRequestedCallback(WindowCloseRequestedCallbackFunc callback);
		void SetWindowClosedCallback(WindowClosedCallbackFunc callback);
		void SetWindowMaximizedCallback(WindowMaximizedCallbackFunc callback);
		void SetWindowMinimizedCallback(WindowMinimizedCallbackFunc callback);
		void SetWindowFocusedCallback(WindowFocusedCallbackFunc callback);
		void SetKeyCallback(KeyCallbackFunc callback);
		void SetKeyTypedCallback(KeyTypedCallbackFunc callback);
		void SetMouseMovedCallback(MouseMovedCallbackFunc callback);
		void SetMouseMovedRelativeCallback(MouseMovedRelativeCallbackFunc callback);
		void SetMouseEnteredCallback(MouseEnteredCallbackFunc callback);

		void SetLogCallback(LogCallbackFunc callback);

		virtual void Update() = 0;

		virtual void* GetNativeHandle() = 0;

		const String& GetAppName() const;
		const String& GetAppVersion() const;
		WindowAPI GetWindowAPI() const;
		LogCallbackFunc GetLogCallback() const;
	};

	QUARTZAPP_API Application* CreateApplication(const ApplicationInfo& appInfo);
	QUARTZAPP_API void DestroyApplication(Application* pApp);
}