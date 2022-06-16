#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"

struct GLFWwindow;

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY);
void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int mazimized);
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

		static void UpdateAndPollMessages();

		static void CallWindowSizeCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int width, int height);

		static void CallWindowPosCallback(GLFWApplication* pApplication,
			GLFWWindow* pWindow, int posX, int posY);

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