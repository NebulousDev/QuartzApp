#pragma once

#include "WinApi.h"
#include "Types/Array.h"
#include "Types/Map.h"
#include "Math/Point.h"

#include "Log.h"

#ifdef QUARTZAPP_GLEW
#include "OpenGL/GLSurface.h"
#endif

#ifdef QUARTZAPP_VULKAN
#include <Vulkan/vulkan.h>
#include "Vulkan/VulkanSurface.h"
#endif

namespace Quartz
{
	class WinApiApplication;
	class WinApiWindow;

	enum
	{
		WINAPI_CLOSE_REQUEST_PARAM = -1
	};

	class WinApiHelper
	{
	private:
		static bool  smInitialized;
		static Vec2u smMonitorSize;
		static uSize smMonitorRefreshRate;
		static Map<int, uSize> smRepeatCounts;

	public:
		static void InitializeWinApi(LogCallbackFunc logCallback);
		static bool IsWinApiInitialized();

		static Surface* CreateSurface(HINSTANCE instance, HWND hwnd, const SurfaceInfo& info, 
			LogCallbackFunc logCallback);

#ifdef QUARTZAPP_GLEW
		static GLSurface* CreateWinApiGLSurface(LogCallbackFunc logCallback);
#endif

#ifdef QUARTZAPP_VULKAN
		static VulkanSurface* CreateWinApiVulkanSurface(HINSTANCE instance, HWND hwnd, 
			const SurfaceInfo& info, LogCallbackFunc logCallback);
#endif

		static void QueryDisplayDevices(Array<DISPLAY_DEVICEW>& devices);
		static void QueryDisplayModes(LPWSTR deviceName, Array<DEVMODEW>& modes);
		static bool FindDisplayMode(LPWSTR deviceName, const Array<DEVMODEW>& modes,
			uSize width, uSize height, uSize refreshRate, DEVMODEW& foundMode);

		static Vec2u GetCurrentMonitorSize();
		static uSize GetCurrentMonitorRefreshRate();

		static void SetDefaultMonitorInfo(Vec2u size, uSize refreshRate);
		static Vec2u GetDefaultMonitorSize();
		static uSize GetDefaultMonitorRefreshRate();

		static void SetWindowOpenState(WinApiWindow* pWindow, bool open);
		static void SetWindowFullscreenState(WinApiWindow* pWindow, bool fullscreen);

		static bool GetWindowLastMinimized(WinApiWindow* pWindow);
		static void SetWindowLastMinimized(WinApiWindow* pWindow, bool lastMinimized);
		static bool GetWindowLastMaximized(WinApiWindow* pWindow);
		static void SetWindowLastMaximized(WinApiWindow* pWindow, bool lastMaximized);

		static bool IsMouseInside(WinApiWindow* pWindow);
		static void SetMouseInside(WinApiWindow* pWindow, bool inside);

		static Point2i GetLastMousePos(WinApiWindow* pWindow);
		static void SetLastMousePos(WinApiWindow* pWindow, const Point2i& position);

		static bool SetDisplayMode(uSize monitor, uSize width, uSize height, 
			uSize refreshRate, LogCallbackFunc logCallback);

		static uSize GetKeyRepeatCount(int scancode);
		static void SetKeyRepeatCount(int scancode, uSize count);

		static void CallWindowSizeCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, int width, int height);

		static void CallWindowPosCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, int posX, int posY);

		static bool CallWindowCloseRequestedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow);

		static void CallWindowClosedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow);

		static void CallWindowMaximizedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, int maximized);

		static void CallWindowMinimizedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, int minimized);

		static void CallWindowFocusedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, int focused);

		static void CallKeyCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, uInt16 scancode, bool down, bool repeat);

		static void CallKeyTypedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, char character, bool repeat);

		static void CallMouseMovedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, uSize mouseX, uSize mouseY);

		static void CallMouseMovedRelativeCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, uSize relX, uSize relY);

		static void CallMouseEnteredCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, bool entered);

		static void PrintLastError(LogCallbackFunc logCallback);
	};
}