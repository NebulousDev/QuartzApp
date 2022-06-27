#include "WinApiHelper.h"

#include "WinApiApplication.h"
#include "WinApiWindow.h"
#include "Types/Array.h"

#include <Vulkan/vulkan_win32.h>
#include <Windows.h>
#include <cstdio>

namespace Quartz
{
	bool  WinApiHelper::smInitialized = false;
	Vec2u WinApiHelper::smMonitorSize = {0,0};
	uSize WinApiHelper::smMonitorRefreshRate = 0;
	Map<int, uSize> WinApiHelper::smRepeatCounts;

#ifdef QUARTZAPP_VULKAN

	void WinApiHelper::InitializeWinApi()
	{
		smMonitorSize = GetCurrentMonitorSize();
		smMonitorRefreshRate = GetCurrentMonitorRefreshRate();
		smInitialized = true;
	}

	bool WinApiHelper::IsWinApiInitialized()
	{
		return smInitialized;
	}

	Surface* WinApiHelper::CreateSurface(HINSTANCE instance, HWND hwnd, const SurfaceInfo& info)
	{
		Surface* pSurface = nullptr;

		switch (info.surfaceApi)
		{
			case SURFACE_API_NONE:
			{
				return pSurface;
			}

			case SURFACE_API_OPENGL:
			{
#ifdef QUARTZAPP_GLEW
				pSurface = WinApiHelper::CreateWinApiGLFWGLSurface();
#else
				printf("Error creating Windows GL Surface: GLEW is not available.\n");
#endif
				return pSurface;
			}

			case SURFACE_API_VULKAN:
			{

#ifdef QUARTZAPP_VULKAN
				pSurface = WinApiHelper::CreateWinApiVulkanSurface(instance, hwnd, info);
#else
				printf("Error creating Windows Vulkan Surface: Vulkan is not available.\n");
#endif
				return pSurface;
			}

			default:
			{
				printf("Error creating Surface: Invalid SurfaceAPI enum.\n");
				return nullptr;
			}
		}
	}

	GLSurface* WinApiHelper::CreateWinApiGLFWGLSurface()
	{
		return nullptr;
	}

	VulkanSurface* WinApiHelper::CreateWinApiVulkanSurface(HINSTANCE instance, HWND hwnd, const SurfaceInfo& info)
	{
		VkSurfaceKHR				vkSurface;
		Array<VkSurfaceFormatKHR>	supportedFormats;
		VkSurfaceCapabilitiesKHR	surfaceCapibilites = {};

		VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = {};
		win32SurfaceInfo.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		win32SurfaceInfo.hinstance	= instance;
		win32SurfaceInfo.hwnd		= hwnd;

		VulkanSurfaceInfo* pVulkanSurfaceInfo = static_cast<VulkanSurfaceInfo*>(info.pApiInfo);

		if (!pVulkanSurfaceInfo)
		{
			printf("Error creating Vulkan Surface: SurfaceInfo->pApiInfo is null.\n");
			return nullptr;
		}

		VkInstance vkInstance = pVulkanSurfaceInfo->instance;
		VkResult result = vkCreateWin32SurfaceKHR(vkInstance, &win32SurfaceInfo, nullptr, &vkSurface);

		if (result != VK_SUCCESS)
		{
			printf("Error creating Vulkan Surface: vkCreateWin32SurfaceKHR() failed.\n");
			return nullptr;
		}

		return new VulkanSurface(vkInstance, vkSurface);
	}

#endif

	void WinApiHelper::QueryDisplayDevices(Array<DISPLAY_DEVICEW>& devices)
	{
		uInt64 devNum = 0;
		DISPLAY_DEVICEW displayDevice = {};

		displayDevice.cb = sizeof(DISPLAY_DEVICEW);

		while(EnumDisplayDevicesW(NULL, devNum, &displayDevice, NULL))
		{
			devices.PushBack(displayDevice);
			++devNum;
		}
	}

	void WinApiHelper::QueryDisplayModes(LPWSTR deviceName, Array<DEVMODEW>& modes)
	{
		uInt64 modeNum = 0;
		DEVMODEW devmode;

		while (EnumDisplaySettingsW(deviceName, modeNum, &devmode))
		{
			modes.PushBack(devmode);
			++modeNum;
		}
	}

	bool WinApiHelper::FindDisplayMode(LPWSTR deviceName, const Array<DEVMODEW>& modes, 
		uSize width, uSize height, uSize refreshRate, DEVMODEW& foundMode)
	{
		Array<DEVMODEW> foundModes;

		for (uSize i = 0; i < modes.Size(); i++)
		{
			const DEVMODEW& devMode = modes[i];

			// Skip all low-bit modes
			if (devMode.dmBitsPerPel < 32) continue;

			// Skip all modes not running our refresh rate
			if (devMode.dmDisplayFrequency != refreshRate) continue;

			// Skip all modes of other sizes than ours
			if (devMode.dmPelsWidth != width ||
				devMode.dmPelsHeight != height)
				continue;

			// Test if this mode will succeed
			if (ChangeDisplaySettingsExW(deviceName, &const_cast<DEVMODEW&>(devMode), 
				NULL, CDS_TEST, NULL) != DISP_CHANGE_SUCCESSFUL)
				continue;

			foundModes.PushBack(devMode);
		}

		if (foundModes.IsEmpty())
		{
			return false;
		}

		// Use first of remaining modes
		foundMode = foundModes[0];

		return true;
	}

	Vec2u WinApiHelper::GetCurrentMonitorSize()
	{
		uSize width = GetSystemMetrics(SM_CXSCREEN);
		uSize height = GetSystemMetrics(SM_CYSCREEN);

		return Vec2u(width, height);
	}

	uSize WinApiHelper::GetCurrentMonitorRefreshRate()
	{
		DEVMODEA devmode = {};
		uSize refreshRate = 60; // default to 60hz

		if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &devmode))
		{
			refreshRate = devmode.dmDisplayFrequency;
		}

		return refreshRate;
	}

	void WinApiHelper::SetDefaultMonitorInfo(Vec2u size, uSize refreshRate)
	{
		smMonitorSize = size;
		smMonitorRefreshRate = refreshRate;
	}

	Vec2u WinApiHelper::GetDefaultMonitorSize()
	{
		return smMonitorSize;
	}

	uSize WinApiHelper::GetDefaultMonitorRefreshRate()
	{
		return smMonitorRefreshRate;
	}

	void WinApiHelper::SetWindowOpenState(WinApiWindow* pWindow, bool open)
	{
		pWindow->mOpen = open;
	}

	void WinApiHelper::SetWindowFullscreenState(WinApiWindow* pWindow, bool fullscreen)
	{
		pWindow->mFullscreen = fullscreen;
	}

	bool WinApiHelper::GetWindowLastMinimized(WinApiWindow* pWindow)
	{
		return pWindow->mLastMinimized;
	}

	void WinApiHelper::SetWindowLastMinimized(WinApiWindow* pWindow, bool lastMinimized)
	{
		pWindow->mLastMinimized = lastMinimized;
	}

	bool WinApiHelper::GetWindowLastMaximized(WinApiWindow* pWindow)
	{
		return pWindow->mLastMaximized;
	}

	void WinApiHelper::SetWindowLastMaximized(WinApiWindow* pWindow, bool lastMaximized)
	{
		pWindow->mLastMaximized = lastMaximized;
	}

	bool WinApiHelper::IsMouseInside(WinApiWindow* pWindow)
	{
		return pWindow->mMouseInside;
	}

	void WinApiHelper::SetMouseInside(WinApiWindow* pWindow, bool inside)
	{
		pWindow->mMouseInside = inside;
	}

	Point2i WinApiHelper::GetLastMousePos(WinApiWindow* pWindow)
	{
		return pWindow->mLastMouse;
	}

	void WinApiHelper::SetLastMousePos(WinApiWindow* pWindow, const Point2i& position)
	{
		pWindow->mLastMouse = position;
	}

	bool WinApiHelper::SetDisplayMode(uSize monitor, uSize width, uSize height, uSize refreshRate)
	{
		DEVMODEW				devMode;
		DISPLAY_DEVICEW			displayDevice;
		LPWSTR					deviceName;
		Array<DISPLAY_DEVICEW>	allDevices;
		Array<DEVMODEW>			allDevModes;

		QueryDisplayDevices(allDevices);
		
		if (monitor > allDevices.Size())
		{
			printf("Failed to set WinApi display mode: Monitor id [%d] not valid.\n", monitor);
			return false;
		}

		displayDevice = allDevices[monitor];
		deviceName = displayDevice.DeviceName;

		QueryDisplayModes(deviceName, allDevModes);

		if (!FindDisplayMode(deviceName, allDevModes, width, height, refreshRate, devMode))
		{
			printf("Error setting display mode: No valid mode found for %dx%dx%d.\n", width, height, refreshRate);
			return false;
		}

		if (ChangeDisplaySettingsExW(deviceName, &devMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("Error setting display mode: ChangeDisplaySettingsExW() failed.\n");
			return false;
		}

		return true;
	}

	uSize WinApiHelper::GetKeyRepeatCount(int scancode)
	{
		// TODO: Look into unknown values being possibly not zero
		return smRepeatCounts[scancode];
	}

	void WinApiHelper::SetKeyRepeatCount(int scancode, uSize count)
	{
		smRepeatCounts[scancode] = count;
	}

	void WinApiHelper::CallWindowSizeCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, int width, int height)
	{
		if (pApplication->mWindowResizedFunc)
			pApplication->mWindowResizedFunc(pWindow, (uSize)width, (uSize)height);
	}

	void WinApiHelper::CallWindowPosCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, int posX, int posY)
	{
		if (pApplication->mWindowMovedFunc)
			pApplication->mWindowMovedFunc(pWindow, (uSize)posX, (uSize)posY);
	}

	bool WinApiHelper::CallWindowCloseRequestedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow)
	{
		if (pApplication->mWindowCloseRequestedFunc)
			return pApplication->mWindowCloseRequestedFunc(pWindow);

		return true;
	}

	void WinApiHelper::CallWindowClosedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow)
	{
		if (pApplication->mWindowClosedFunc)
			pApplication->mWindowClosedFunc(pWindow);
	}

	void WinApiHelper::CallWindowMaximizedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, int maximized)
	{
		if (pApplication->mWindowMaximizedFunc)
			pApplication->mWindowMaximizedFunc(pWindow, !(bool)maximized);
	}

	void WinApiHelper::CallWindowMinimizedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, int minimized)
	{
		if (pApplication->mWindowMinimizedFunc)
			pApplication->mWindowMinimizedFunc(pWindow, !(bool)minimized);
	}

	void WinApiHelper::CallWindowFocusedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, int focused)
	{
		if (pApplication->mWindowFocusedFunc)
			pApplication->mWindowFocusedFunc(pWindow, !(bool)focused);
	}

	void WinApiHelper::CallKeyCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, uInt16 scancode, bool down, bool repeat)
	{
		if (pApplication->mKeyFunc)
			pApplication->mKeyFunc(pWindow, scancode, down, repeat);
	}

	void WinApiHelper::CallKeyTypedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, char character, bool repeat)
	{
		if (pApplication->mKeyTypedFunc)
			pApplication->mKeyTypedFunc(pWindow, character, repeat);
	}

	void WinApiHelper::CallMouseMovedCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, uSize mouseX, uSize mouseY)
	{
		if (pApplication->mMouseMovedFunc)
			pApplication->mMouseMovedFunc(pWindow, mouseX, mouseY);
	}

	void WinApiHelper::CallMouseMovedRelativeCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, uSize relX, uSize relY)
	{
		if (pApplication->mMouseMovedRelativeFunc)
			pApplication->mMouseMovedRelativeFunc(pWindow, relX, relY);
	}

	void WinApiHelper::CallMouseEnteredCallback(WinApiApplication* pApplication, WinApiWindow* pWindow, bool entered)
	{
		if (pApplication->mMouseEnteredFunc)
			pApplication->mMouseEnteredFunc(pWindow, entered);
	}

	void WinApiHelper::PrintLastError()
	{
		DWORD error = GetLastError();

		LPWSTR pErrorMessage = nullptr;

		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&pErrorMessage,
			0, NULL);

		if (pErrorMessage == nullptr)
		{
			// No error
			return;
		}

		wprintf(pErrorMessage);

		LocalFree(pErrorMessage);
	}
}