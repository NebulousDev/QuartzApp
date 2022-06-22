#include "WinApiRegistry.h"

#include "WinApiApplication.h"

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
	}

	void WinApiRegistry::UnregisterApp(const WinApiApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		smAppCount--;

		if (smAppCount == 0)
		{
			smRegistry.Remove(handle);
		}
	}

	void WinApiRegistry::RegisterAppWindow(const WinApiApplication* pApp, WinApiWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		//glfwSetWindowSizeCallback(pWindow->GetGLFWHandle(), GLFWWindowSizeCallback);
		//glfwSetWindowPosCallback(pWindow->GetGLFWHandle(), GLFWWindowPosCallback);
		//glfwSetWindowMaximizeCallback(pWindow->GetGLFWHandle(), GLFWWindowMaximizedCallback);
		//glfwSetWindowIconifyCallback(pWindow->GetGLFWHandle(), GLFWWindowMinimzedCallback);
		//glfwSetWindowFocusCallback(pWindow->GetGLFWHandle(), GLFWWindowFocusedCallback);

		// Assure app is registered
		assert(smRegistry.Contains(handle));

		smRegistry[handle].PushBack(pWindow);
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