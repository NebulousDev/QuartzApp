#include "GLFWApplication.h"

#include "GLFWWindow.h"
#include "GLFWHelper.h"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace Quartz
{
	GLFWApplication::GLFWApplication(const ApplicationInfo& appInfo)
		: Application(appInfo) { }

	bool GLFWApplication::Create()
	{
		if (!GLFWHelper::IsGLFWInitialized())
		{
			if (!GLFWHelper::InitializeGLFW())
			{
				return false;
			}
		}

		GLFWHelper::RegisterApp(this);

		return true;
	}

	void GLFWApplication::Destroy()
	{
		// Intentional copy so as to not remove elements from the itterator live
		Array<GLFWWindow*> appWindows = GLFWHelper::GetWindows(this);

		for (GLFWWindow* pWindow : appWindows)
		{
			pWindow->Destroy();
		}

		GLFWHelper::UnregisterApp(this);

		if (GLFWHelper::AppCount() == 0)
		{
			glfwTerminate();
		}
	}

	Window* GLFWApplication::CreateWindow(const WindowInfo& info)
	{
		Window* pWindow = new GLFWWindow(info, this);

		if (!pWindow->Create())
		{
			delete pWindow;

			return nullptr;
		}

		return pWindow;
	}

	void GLFWApplication::DestroyWindow(Window* pWindow)
	{
		pWindow->Destroy();
	}

	void GLFWApplication::Update()
	{
		// Note that this will be called for every Application which calls Update()
		GLFWHelper::UpdateAndPollMessages();
	}

	void* GLFWApplication::GetNativeHandle()
	{
		// GLFW does not have a native application handle
		return nullptr;
	}
}

