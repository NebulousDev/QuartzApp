#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"
#include "Math/Point.h"

#include "LogCallbacks.h"

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
		static bool InitializeGLFW(LogCallbackFunc logCallback);
		static bool IsGLFWInitialized();

		static void SetWindowState(GLFWWindow* pWindow, GLFWWindowState state);

		static Point2i GetLastMousePos(GLFWWindow* pWindow);
		static void SetLastMousePos(GLFWWindow* pWindow, const Point2i& position);

#ifdef QUARTZAPP_GLEW
		static GLSurface* CreateGLFWGLSurface(LogCallbackFunc logCallback);
		static void DestroyGLFWGLSurface(GLSurface* pSurface);
#endif

#ifdef QUARTZAPP_VULKAN
		static VulkanSurface* CreateGLFWVulkanSurface(GLFWwindow* pGLFWwindow, const SurfaceInfo& info, LogCallbackFunc logCallback);
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
			GLFWWindow* pWindow, int scancode, bool down, bool repeat);

		static void CallKeyTypedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int codepoint, bool repeat);

		static void CallMouseMovedCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, double mouseX, double mouseY);

		static void CallMouseMovedRelativeCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, double relX, double relY);

		static void CallMouseEnteredCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int entered);

		static void PrintError(LogCallbackFunc logCallback);
	};
}