#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"

#include "GLFWWindow.h"
#include "GLFWCallbacks.h"

#ifdef QUARTZAPP_GLEW
#include "OpenGL/GLSurface.h"
#endif

#ifdef QUARTZAPP_VULKAN
#include "Vulkan/VulkanSurface.h"
#endif

namespace Quartz
{
	class GLFWApplication;
	class GLFWWindow;

	class GLFWHelper
	{
	private:
		static bool				smInitialzed;
		static Map<int, bool>	smRepeatMap;

	public:
		static bool InitializeGLFW();
		static bool IsGLFWInitialized();

		friend void ::GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
		friend void ::GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY);
		friend bool ::GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow);
		friend void ::GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
		friend void ::GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int mazimized);
		friend void ::GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized);
		friend void ::GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused);
		friend void ::GLFWKeyCallback(GLFWwindow* pGLFWwindow, int key, int scancode, int action, int mods);
		friend void ::GLFWKeyTypedCallback(GLFWwindow* pGLFWwindow, unsigned int codepoint);

		static void SetWindowState(GLFWWindow* pWindow, GLFWWindowState state);

#ifdef QUARTZAPP_GLEW
		static GLSurface* CreateGLFWGLSurface();
		static void DestroyGLFWGLSurface(GLSurface* pSurface);
#endif

#ifdef QUARTZAPP_VULKAN
		static VulkanSurface* CreateGLFWVulkanSurface(GLFWwindow* pGLFWwindow, const SurfaceInfo& info);
		static void DestroyGLFWVulkanSurface(VulkanSurface* pSurface);
#endif

		static bool IsKeyRepeating(int codepoint);
		static void SetKeyRepeating(int codepoint, bool repeat);

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

		static void CallKeyCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, uInt16 scancode, bool down, bool repeat);

		static void CallKeyTypedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, char character, bool repeat);

		static void PrintError();
	};
}