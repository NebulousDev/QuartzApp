#include "GLFWRegistry.h"

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
	}

	void GLFWRegistry::UnregisterApp(const GLFWApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		smAppCount--;

		if (smAppCount == 0)
		{
			smRegistry.Remove(handle);
		}
	}

	void GLFWRegistry::RegisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		glfwSetWindowSizeCallback(pWindow->GetGLFWHandle(), GLFWWindowSizeCallback);
		glfwSetWindowPosCallback(pWindow->GetGLFWHandle(), GLFWWindowPosCallback);
		glfwSetWindowMaximizeCallback(pWindow->GetGLFWHandle(), GLFWWindowMaximizedCallback);
		glfwSetWindowIconifyCallback(pWindow->GetGLFWHandle(), GLFWWindowMinimzedCallback);
		glfwSetWindowFocusCallback(pWindow->GetGLFWHandle(), GLFWWindowFocusedCallback);
		glfwSetKeyCallback(pWindow->GetGLFWHandle(), GLFWKeyCallback);
		glfwSetCharCallback(pWindow->GetGLFWHandle(), GLFWKeyTypedCallback);

		// Assure app is registered
		assert(smRegistry.Contains(handle));

		smRegistry[handle].PushBack(pWindow);
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