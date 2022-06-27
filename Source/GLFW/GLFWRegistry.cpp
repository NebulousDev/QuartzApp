#include "GLFWRegistry.h"

#include "Log.h"

#include "GLFWApplication.h"
#include "GLFWWindow.h"
#include "GLFWCallbacks.h"
#include <GLFW/glfw3.h>

#include <cassert>

namespace Quartz
{
	uSize								GLFWRegistry::smAppCount = 0;
	Map<handle64, Array<GLFWWindow*>>	GLFWRegistry::smRegistry;
	
	void GLFWRegistry::RegisterApp(const GLFWApplication* pApp)
	{
		smAppCount++;

		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Construct blank entry
		smRegistry.Put(handle);

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE, 
			"QuartzApp: Registered GLFW Application (%s, %s) with handle [%p].", 
			pApp->GetAppName().Str(), pApp->GetAppVersion().Str(), handle);
	}

	void GLFWRegistry::UnregisterApp(const GLFWApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		smAppCount--;

		// TODO: revisit this?
		if (smAppCount == 0)
		{
			smRegistry.Remove(handle);
		}

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE, 
			"QuartzApp: Unregistered GLFW Application (%s, %s) with handle [%p].", 
			pApp->GetAppName().Str(), pApp->GetAppVersion().Str(), handle);
	}

	void GLFWRegistry::RegisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Assure app is registered
		assert(smRegistry.Contains(handle));

		smRegistry[handle].PushBack(pWindow);

		AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE, 
			"QuartzApp: Registered GLFW Window (%s) with handle [%p].", pWindow->GetTitle().Str(), handle);
	}

	void GLFWRegistry::UnregisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);
		Map<handle64, Array<GLFWWindow*>>::Iterator appItr = smRegistry.Find(handle);

		if (appItr != smRegistry.End())
		{
			Array<GLFWWindow*>& windows = appItr->value;
			auto windowItr = windows.Find(pWindow);

			if (windowItr != windows.End())
			{
				windows.Remove(windowItr);

				AppLogCallback(pApp->GetLogCallback(), LOG_LEVEL_TRACE,
					"QuartzApp: Unregistered GLFW Window (%s) with handle [%p].", pWindow->GetTitle().Str(), handle);
			}
		}
	}

	Array<GLFWWindow*>& GLFWRegistry::GetWindows(const GLFWApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Assure the application is valid, otherwise a false entry will be created.
		assert(smRegistry.Contains(handle));

		return smRegistry[handle];
	}

	uSize GLFWRegistry::AppCount()
	{
		return smAppCount;
	}
}