#pragma once

#include "WinApi.h"
#include "Types/Array.h"
#include "Types/Map.h"
#include "Math/Vector.h"

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
		static void InitializeWinApi();
		static bool IsWinApiInitialized();

		static Surface* CreateSurface(HINSTANCE instance, HWND hwnd, const SurfaceInfo& info);

#ifdef QUARTZAPP_GLEW
		static GLSurface* CreateWinApiGLFWGLSurface();
#endif

#ifdef QUARTZAPP_VULKAN
		static VulkanSurface* CreateWinApiVulkanSurface(
			HINSTANCE instance, HWND hwnd, const SurfaceInfo& info);
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

		static bool SetDisplayMode(uSize monitor, uSize width, uSize height, uSize refreshRate);

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

		static void CallWindowKeyCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, uInt16 scancode, bool down, bool repeat);

		static void CallWindowKeyTypedCallback(WinApiApplication* pApplication,
			WinApiWindow* pWindow, char character, bool repeat);

		static void PrintLastError();
	};
}