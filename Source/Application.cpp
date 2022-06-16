#include "Application.h"
#include "GLFW/GLFWApplication.h"

namespace Quartz
{
	Application::Application(const ApplicationInfo& appInfo) :
		mAppName(appInfo.appName),
		mVersion(appInfo.version),
		mWindowResizedFunc(nullptr),
		mWindowMovedFunc(nullptr),
		mWindowClosedFunc(nullptr),
		mWindowMaximizedFunc(nullptr),
		mWindowMinimizedFunc(nullptr),
		mWindowFocusedFunc(nullptr)
	{ }

	void Application::SetWindowResizedCallback(WindowResizedCallbackFunc callback)
	{
		mWindowResizedFunc = callback;
	}

	void Application::SetWindowMovedCallback(WindowMovedCallbackFunc callback)
	{
		mWindowMovedFunc = callback;
	}

	void Application::SetWindowClosedCallback(WindowClosedCallbackFunc callback)
	{
		mWindowClosedFunc = callback;
	}

	void Application::SetWindowMaximizedCallback(WindowMaximizedCallbackFunc callback)
	{
		mWindowMaximizedFunc = callback;
	}

	void Application::SetWindowMinimizedCallback(WindowMinimizedCallbackFunc callback)
	{
		mWindowMinimizedFunc = callback;
	}

	void Application::SetWindowFocusedCallback(WindowFocusedCallbackFunc callback)
	{
		mWindowFocusedFunc = callback;
	}

	Application* CreateApplication(const ApplicationInfo& appInfo)
	{
		GLFWApplication* pApplication = new GLFWApplication(appInfo);

		pApplication->Create();

		return pApplication;
	}

	void DestroyApplication(Application* pApp)
	{
		pApp->Destroy();
	}
}


