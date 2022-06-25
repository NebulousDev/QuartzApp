#include "GLFWHelper.h"

#include "GLFWApplication.h"
#include "GLFWWindow.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cassert>

namespace Quartz
{
	bool GLFWHelper::smInitialzed = false;
	Map<int, bool> GLFWHelper::smRepeatMap;

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
		VulkanSurfaceInfo* pApiInfo = static_cast<VulkanSurfaceInfo*>(info.pApiInfo);

		if (pApiInfo == nullptr)
		{
			printf("Failed to create (GLFW) VulkanSurface: Invalid apiInfo.");
			return nullptr;
		}

		VkSurfaceKHR vkSurface;
		VkResult error = glfwCreateWindowSurface(pApiInfo->instance, pGLFWwindow, nullptr, &vkSurface);

		if (error)
		{
			PrintError();
			return nullptr;
		}

		return new VulkanSurface(pApiInfo->instance, vkSurface);
	}

	void GLFWHelper::DestroyGLFWVulkanSurface(VulkanSurface* pSurface)
	{
		vkDestroySurfaceKHR(pSurface->GetVkInsance(), pSurface->GetVkSurface(), nullptr);
		delete pSurface;
	}

#endif

	bool GLFWHelper::IsKeyRepeating(int codepoint)
	{
		// TODO: Look into unknown values being possibly not false
		return smRepeatMap[codepoint];
	}

	void GLFWHelper::SetKeyRepeating(int codepoint, bool repeat)
	{
		smRepeatMap[codepoint] = repeat;
	}

	void GLFWHelper::CallWindowSizeCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int width, int height)
	{
		if (pApplication->mWindowResizedFunc)
			pApplication->mWindowResizedFunc(pWindow, (uSize)width, (uSize)height);
	}

	void GLFWHelper::CallWindowPosCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int posX, int posY)
	{
		if (pApplication->mWindowMovedFunc)
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

	void GLFWHelper::CallKeyCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, uInt16 scancode, bool down, bool repeat)
	{
		if (pApplication->mKeyFunc)
			pApplication->mKeyFunc(pWindow, scancode, down, repeat);
	}

	void GLFWHelper::CallKeyTypedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, char character, bool repeat)
	{
		if (pApplication->mKeyTypedFunc)
			pApplication->mKeyTypedFunc(pWindow, character, repeat);
	}

	void GLFWHelper::PrintError()
	{
		const char* glfwError;

		int error = glfwGetError(&glfwError);
		printf("GLFW fatal error: [%d]\n%s\n", error, glfwError);
	}
}