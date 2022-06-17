#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"

#include "GLFWWindow.h"

#ifdef QUARTZAPP_GLEW
#include "OpenGL/GLSurface.h"
#endif

#ifdef QUARTZAPP_VULKAN
#include "Vulkan/VulkanSurface.h"
#endif

struct GLFWwindow;

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY);
void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
bool GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow);
void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int maximized);
void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized);
void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused);

namespace Quartz
{
	class GLFWApplication;
	class GLFWWindow;

	class GLFWHelper
	{
	private:
		static uSize smAppCount;
		static Map<handle64, Array<GLFWWindow*>> smRegistry;
		static bool smInitialzed;

		friend void ::GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
		friend void ::GLFWWindowPosCallback(GLFWwindow * pGLFWwindow, int posX, int posY);
		friend bool ::GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow);
		friend void ::GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
		friend void ::GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int mazimized);
		friend void ::GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized);
		friend void ::GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused);

	public:
		static bool InitializeGLFW();
		static bool IsGLFWInitialized();

		static void RegisterApp(const GLFWApplication* pApp);
		static void UnregisterApp(const GLFWApplication* pApp);

		static void RegisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow);
		static void UnregisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow);

		static void SetWindowState(GLFWWindow* pWindow, GLFWWindowState state);

#ifdef QUARTZAPP_GLEW
		static GLSurface* CreateGLFWGLSurface();
		static void DestroyGLFWGLSurface(GLSurface* pSurface);
#endif

#ifdef QUARTZAPP_VULKAN
		static VulkanSurface* CreateGLFWVulkanSurface(GLFWwindow* pGLFWwindow, const SurfaceInfo& info);
		static void DestroyGLFWVulkanSurface(VulkanSurface* pSurface);
#endif

		static void CallWindowSizeCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int width, int height);

		static void CallWindowPosCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int posX, int posY);

		static bool CallWindowCloseRequestedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow);

		static void CallWindowClosedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow);

		static void CallWindowMaximizedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int maximized);

		static void CallWindowMinimizedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int minimized);

		static void CallWindowFocusedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int focused);

		static void PrintError();

		static Array<GLFWWindow*>& GetWindows(const GLFWApplication* pApp);
		static uSize AppCount();
	};
}