#pragma once

#include "WinApi.h"
#include "Types/Array.h"

#ifdef QUARTZAPP_GLEW
#include "OpenGL/GLSurface.h"
#endif

#ifdef QUARTZAPP_VULKAN
#include <Vulkan/vulkan.h>
#include "Vulkan/VulkanSurface.h"
#endif

namespace Quartz
{
	class WinApiWindow;

	class WinApiHelper
	{
	public:
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

		static void SetWindowOpenState(WinApiWindow* pWindow, bool open);
		static void SetWindowFullscreenState(WinApiWindow* pWindow, bool fullscreen);

		static bool SetDisplayMode(uSize monitor, uSize width, uSize height, uSize refreshRate);

		static void PrintLastError();
	};
}