#include "WinApiRegistry.h"

#include "Log.h"

#include "WinApiApplication.h"
#include "WinApiWindow.h"

#include <cassert>

namespace Quartz
{
	uSize								WinApiRegistry::smAppCount = 0;
	Map<handle64, Array<WinApiWindow*>>	WinApiRegistry::smRegistry;

	void WinApiRegistry::RegisterApp(const WinApiApplication* pApp)
	{
		smAppCount++;

		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Construct blank entry
		smRegistry.Put(handle);

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE,
			"QuartzApp: Registered WinApi Application (%s, %s) with handle [%p].", 
			pApp->GetAppName().Str(), pApp->GetAppVersion().Str(), handle);
	}

	void WinApiRegistry::UnregisterApp(const WinApiApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		smAppCount--;

		if (smAppCount == 0)
		{
			smRegistry.Remove(handle);
		}

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE,
			"QuartzApp: Unregistered WinApi Application (%s, %s) with handle [%p].", 
			pApp->GetAppName().Str(), pApp->GetAppVersion().Str(), handle);
	}

	void WinApiRegistry::RegisterAppWindow(const WinApiApplication* pApp, WinApiWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Assure app is registered
		assert(smRegistry.Contains(handle));

		smRegistry[handle].PushBack(pWindow);

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE,
			"QuartzApp: Registered WinApi Window (%s) with handle [%p].", pWindow->GetTitle().Str(), handle);
	}

	void WinApiRegistry::UnregisterAppWindow(const WinApiApplication* pApp, WinApiWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);
		Map<handle64, Array<WinApiWindow*>>::Iterator appItr = smRegistry.Find(handle);

		if (appItr != smRegistry.End())
		{
			Array<WinApiWindow*>& windows = appItr->value;
			auto windowItr = windows.Find(pWindow);

			if (windowItr != windows.End())
			{
				windows.Remove(windowItr);

				AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE,
					"QuartzApp: Unregistered WinApi Window (%s) with handle [%p].", pWindow->GetTitle().Str(), handle);
			}
		}
	}

	Array<WinApiWindow*>& WinApiRegistry::GetWindows(const WinApiApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Assure the application is valid, otherwise a false entry will be created.
		assert(smRegistry.Contains(handle));

		return smRegistry[handle];
	}

	uSize WinApiRegistry::AppCount()
	{
		return smAppCount;
	}
}