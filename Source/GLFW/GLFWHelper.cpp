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
		glfwSetWindowMaximizeCallback(pWindow->mpGLFWwindow, GLFWWindowMaximizedCallback);
		glfwSetWindowIconifyCallback(pWindow->mpGLFWwindow, GLFWWindowMinimzedCallback);
		glfwSetWindowFocusCallback(pWindow->mpGLFWwindow, GLFWWindowFocusedCallback);

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

	void GLFWHelper::SetWindowState(GLFWWindow* pWindow, GLFWWindowState state)
	{
		pWindow->mWindowState = state;
	}

#ifdef QUARTZAPP_GLEW

	GLSurface* GLFWHelper::CreateGLFWGLSurface()
	{
		return new GLSurface();
	}

	void GLFWHelper::DestroyGLFWGLSurface(GLSurface* pSurface)
	{
		delete pSurface;
	}

#endif

#ifdef QUARTZAPP_VULKAN

	VulkanSurface* GLFWHelper::CreateGLFWVulkanSurface(GLFWwindow* pGLFWwindow, const SurfaceInfo& info)
	{
		VulkanSurfaceApiInfo* pApiInfo = static_cast<VulkanSurfaceApiInfo*>(info.pApiInfo);

		if (pApiInfo == nullptr)
		{
			printf("Failed to create (GLFW) VulkanSurface: Invalid apiInfo.");
			return nullptr;
		}

		VkSurfaceKHR vkSurface;
		VkResult error = glfwCreateWindowSurface(pApiInfo->vkInstance, pGLFWwindow, nullptr, &vkSurface);

		if (error)
		{
			PrintError();
			return nullptr;
		}

		return new VulkanSurface(pApiInfo->vkInstance, vkSurface);
	}

	void GLFWHelper::DestroyGLFWVulkanSurface(VulkanSurface* pSurface)
	{
		vkDestroySurfaceKHR(pSurface->GetVkInsance(), pSurface->GetVkSurface(), nullptr);
		delete pSurface;
	}

#endif

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

	bool GLFWHelper::CallWindowCloseRequestedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow)
	{
		if (pApplication->mWindowCloseRequestedFunc)
			return pApplication->mWindowCloseRequestedFunc(pWindow);

		return true;
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
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowClosedCallback(pApp, pWindow);
}

bool GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	return GLFWHelper::CallWindowCloseRequestedCallback(pApp, pWindow);
}

void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int maximized)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowMaximizedCallback(pApp, pWindow, maximized);
}

void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowMinimizedCallback(pApp, pWindow, minimized);
}

void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowFocusedCallback(pApp, pWindow, focused);
}
