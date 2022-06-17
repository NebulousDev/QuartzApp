#include "Application.h"
#include "GLFW/GLFWApplication.h"

namespace Quartz
{
	Application::Application(const ApplicationInfo& appInfo) :
		mAppName(appInfo.appName),
		mVersion(appInfo.version),
		mWindowAPI(WINDOW_API_NONE),
		mWindowResizedFunc(nullptr),
		mWindowMovedFunc(nullptr),
		mWindowClosedFunc(nullptr),
		mWindowCloseRequestedFunc(nullptr),
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

	void Application::SetWindowCloseRequestedCallback(WindowCloseRequestedCallbackFunc callback)
	{
		mWindowCloseRequestedFunc = callback;
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
		Application* pApplication = nullptr;

		switch (appInfo.windowApi)
		{
			case WINDOW_API_GLFW:
			{
#ifdef QUARTZAPP_GLFW
				pApplication = new GLFWApplication(appInfo);
				break;
#else
				printf("Error creating GLFW Application: GLFW is not available.");
				return nullptr;
#endif
			}

			case WINDOW_API_WINAPI:
			{
#ifdef QUARTZAPP_GLFW
				pApplication = new GLFWApplication(appInfo);
				break;
#else
				printf("Error creating WinApi Application: WinApi is not available.");
				return nullptr;
#endif
			}

			default:
			{
				printf("Error creating Application: Invalid WindowAPI enum.");
				return nullptr;
			}
		}

		if (!pApplication->Create())
		{
			return nullptr;
		}

		return pApplication;
	}

	void DestroyApplication(Application* pApp)
	{
		pApp->Destroy();
		delete pApp;
	}
}


