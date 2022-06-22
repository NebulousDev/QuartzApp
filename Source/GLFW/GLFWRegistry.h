#pragma once

#include "Types/Types.h"
#include "Types/Map.h"
#include "Types/Array.h"

namespace Quartz
{
	class GLFWApplication;
	class GLFWWindow;

	class GLFWRegistry
	{
	private:
		static uSize smAppCount;
		static Map<handle64, Array<GLFWWindow*>> smRegistry;

	public:
		static void RegisterApp(const GLFWApplication* pApp);
		static void UnregisterApp(const GLFWApplication* pApp);

		static void RegisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow);
		static void UnregisterAppWindow(const GLFWApplication* pApp, GLFWWindow* pWindow);

		static Array<GLFWWindow*>& GetWindows(const GLFWApplication* pApp);
		static uSize AppCount();
	};
}