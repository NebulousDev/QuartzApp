#include "GLFWHelper.h"

#include "GLFWApplication.h"
#include "GLFWWindow.h"

#include "Log.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cassert>

namespace Quartz
{
	bool GLFWHelper::smInitialzed = false;
	Map<int, bool> GLFWHelper::smRepeatMap;

	bool GLFWHelper::InitializeGLFW(LogCallbackFunc logCallback)
	{
		if (!glfwInit())
		{
			AppLogCallback(logCallback, LOG_LEVEL_FATAL, "QuartzApp: GLFW failed to initialize.");
			PrintError(logCallback);
			return false;
		}

		smInitialzed = true;

		const char* glfwVersion = glfwGetVersionString();

		AppLogCallback(logCallback, LOG_LEVEL_INFO, "QuartzApp: GLFW initialized successfully.");
		AppLogCallback(logCallback, LOG_LEVEL_INFO, "QuartzApp: GLFW Version %s", glfwVersion);

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

	Point2i GLFWHelper::GetLastMousePos(GLFWWindow* pWindow)
	{
		return pWindow->mLastMouse;
	}

	void GLFWHelper::SetLastMousePos(GLFWWindow* pWindow, const Point2i& position)
	{
		pWindow->mLastMouse = position;
	}

#ifdef QUARTZAPP_GLEW

	GLApiSurface* GLFWHelper::CreateGLFWGLSurface(LogCallbackFunc logCallback)
	{
		return new GLApiSurface();
	}

	void GLFWHelper::DestroyGLFWGLSurface(GLApiSurface* pSurface)
	{
		delete pSurface;
	}

#endif

#ifdef QUARTZAPP_VULKAN

	VulkanApiSurface* GLFWHelper::CreateGLFWVulkanSurface(GLFWwindow* pGLFWwindow, const SurfaceInfo& info, LogCallbackFunc logCallback)
	{
		VulkanApiSurfaceInfo* pApiInfo = static_cast<VulkanApiSurfaceInfo*>(info.pApiInfo);

		if (pApiInfo == nullptr)
		{
			AppLogCallback(logCallback, LOG_LEVEL_ERROR, "QuartzApp: Failed to create GLFW VulkanApiSurface: Invalid apiInfo.");
			return nullptr;
		}

		VkSurfaceKHR vkSurface;
		VkResult error = glfwCreateWindowSurface(pApiInfo->instance, pGLFWwindow, nullptr, &vkSurface);

		if (error)
		{
			PrintError(logCallback);
			return nullptr;
		}

		AppLogCallback(logCallback, LOG_LEVEL_INFO, "QuartzApp: GLFW Vulkan Surface created successfully.");

		return new VulkanApiSurface(pApiInfo->instance, vkSurface);
	}

	void GLFWHelper::DestroyGLFWVulkanSurface(VulkanApiSurface* pSurface)
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

	void GLFWHelper::CallKeyCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int scancode, bool down, bool repeat)
	{
		if (pApplication->mKeyFunc)
			pApplication->mKeyFunc(pWindow, (uInt16)scancode, down, repeat);
	}

	void GLFWHelper::CallKeyTypedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int codepoint, bool repeat)
	{
		if (pApplication->mKeyTypedFunc)
			pApplication->mKeyTypedFunc(pWindow, (char)codepoint, repeat);
	}

	void GLFWHelper::CallMouseMovedCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, double mouseX, double mouseY)
	{
		if (pApplication->mMouseMovedFunc)
			pApplication->mMouseMovedFunc(pWindow, (uSize)mouseX, (uSize)mouseY);
	}

	void GLFWHelper::CallMouseMovedRelativeCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, double relX, double relY)
	{
		if (pApplication->mMouseMovedRelativeFunc)
			pApplication->mMouseMovedRelativeFunc(pWindow, (sSize)relX, (sSize)relY);
	}

	void GLFWHelper::CallMouseEnteredCallback(GLFWApplication* pApplication, GLFWWindow* pWindow, int entered)
	{
		if (pApplication->mMouseEnteredFunc)
			pApplication->mMouseEnteredFunc(pWindow, (bool)entered);
	}

	void GLFWHelper::PrintError(LogCallbackFunc logCallback)
	{
		const char* glfwError;
		int error = glfwGetError(&glfwError);
		AppLogCallback(logCallback, LOG_LEVEL_ERROR, "QuartzApp: GLFW fatal error [%d]: %s", error, glfwError);
	}
}