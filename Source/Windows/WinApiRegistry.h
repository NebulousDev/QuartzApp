#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"

namespace Quartz
{
	class WinApiWindow;
	class WinApiApplication;

	class WinApiRegistry
	{
	private:
		static uSize smAppCount;
		static Map<handle64, Array<WinApiWindow*>> smRegistry;

	public:
		static void RegisterApp(const WinApiApplication* pApp);
		static void UnregisterApp(const WinApiApplication* pApp);

		static void RegisterAppWindow(const WinApiApplication* pApp, WinApiWindow* pWindow);
		static void UnregisterAppWindow(const WinApiApplication* pApp, WinApiWindow* pWindow);

		static Array<WinApiWindow*>& GetWindows(const WinApiApplication* pApp);
		static uSize AppCount();
	};
}