#include "GLFWHelper.h"

#include "GLFWApplication.h"
#include "GLFWWindow.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cassert>

namespace Quartz
{
	bool								GLFWHelper::smInitialzed = false;
	uSize								GLFWHelper::smAppCount = 0;
	Map<handle64, Array<GLFWWindow*>>	GLFWHelper::smRegistry;

	bool GLFWHelper::InitializeGLFW()
	{
		if (!glfwInit())
		{
			printf("GLFW failed to initialize.\n");
			PrintError();
			return false;
		}

		smInitialzed = true;

		const char* glfwVersion = glfwGetVersionString();

		printf("GLFW Initialized Successfully.\n");
		printf("GLFW Version %s\n", glfwVersion);

		return true;
	}

	bool GLFWHelper::IsGLFWInitialized()
	{
		return smInitialzed;
	}

	void GLFWHelper::RegisterApp(const GLFWApplication* pApp)
	{
		smAppCount++;

		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Construct blank entry
		smRegistry.Put(handle);
	}

	void GLFWHelper::UnregisterApp(const GLFWApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		smAppCount--;

		if (smAppCount == 0)
		{
			smRegistry.Remove(handle);
		}
	}

	void GLFWHelper::RegisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		glfwSetWindowSizeCallback(pWindow->mpGLFWwindow, GLFWWindowSizeCallback);
		glfwSetWindowPosCallback(pWindow->mpGLFWwindow, GLFWWindowPosCallback);

		// Assure app is registered
		assert(smRegistry.Contains(handle));
		
		smRegistry[handle].PushBack(pWindow);
	}

	void GLFWHelper::UnregisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow)
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

	void GLFWHelper::UpdateAndPollMessages()
	{
		glfwPollEvents();
	}

	void GLFWHelper::CallWindowSizeCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int width, int height)
	{
		if (pApplication->mWindowResizedFunc)
			pApplication->mWindowResizedFunc(pWindow, (uSize)width, (uSize)height);
	}

	void GLFWHelper::CallWindowPosCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int posX, int posY)
	{
		if(pApplication->mWindowMovedFunc)
			pApplication->mWindowMovedFunc(pWindow, (uSize)posX, (uSize)posY);
	}

	void GLFWHelper::CallWindowClosedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow)
	{
		if (pApplication->mWindowClosedFunc)
			pApplication->mWindowClosedFunc(pWindow);
	}

	void GLFWHelper::CallWindowMaximizedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int maximized)
	{
		if (pApplication->mWindowMaximizedFunc)
			pApplication->mWindowMaximizedFunc(pWindow, !(bool)maximized);
	}

	void GLFWHelper::CallWindowMinimizedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int minimized)
	{
		if (pApplication->mWindowMinimizedFunc)
			pApplication->mWindowMinimizedFunc(pWindow, !(bool)minimized);
	}

	void GLFWHelper::CallWindowFocusedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int focused)
	{
		if (pApplication->mWindowFocusedFunc)
			pApplication->mWindowFocusedFunc(pWindow, !(bool)focused);
	}

	void GLFWHelper::PrintError()
	{
		const char* glfwError;

		int error = glfwGetError(&glfwError);
		printf("GLFW fatal error: [%d]\n%s\n", error, glfwError);
	}

	Array<GLFWWindow*>& GLFWHelper::GetWindows(const GLFWApplication* pApp)
	{
		handle64 handle = reinterpret_cast<handle64>(pApp);

		// Assure the application is valid, otherwise a false entry will be created.
		assert(smRegistry.Contains(handle));

		return smRegistry[handle];
	}

	uSize GLFWHelper::AppCount()
	{
		return smAppCount;
	}
}

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowSizeCallback(pApp, pWindow, width, height);
}

void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowPosCallback(pApp, pWindow, posX, posY);
}

void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow)
{

}

void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int mazimized)
{

}

void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized)
{

}

void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused)
{

}
