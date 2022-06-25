#include "Application.h"

#ifdef QUARTZAPP_GLFW
#include "GLFW/GLFWApplication.h"
#endif

#ifdef QUARTZAPP_WINAPI
#include "Windows/WinApiApplication.h"
#endif

namespace Quartz
{
	Application::Application(const ApplicationInfo& appInfo) :
		mAppName(appInfo.appName),
		mVersion(appInfo.version),
		mWindowAPI(WINDOW_API_NONE),
		mHints(appInfo.hints),
		mWindowResizedFunc(nullptr),
		mWindowMovedFunc(nullptr),
		mWindowClosedFunc(nullptr),
		mWindowCloseRequestedFunc(nullptr),
		mWindowMaximizedFunc(nullptr),
		mWindowMinimizedFunc(nullptr),
		mWindowFocusedFunc(nullptr),
		mKeyFunc(nullptr),
		mKeyTypedFunc(nullptr)
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

	void Application::SetKeyCallback(KeyCallbackFunc callback)
	{
		mKeyFunc = callback;
	}

	void Application::SetKeyTypedCallback(KeyTypedCallbackFunc callback)
	{
		mKeyTypedFunc = callback;
	}

	WindowAPI Application::GetWindowAPI() const
	{
		return mWindowAPI;
	}

	Application* CreateApplication(const ApplicationInfo& appInfo)
	{
		Application* pApplication = nullptr;

		switch (appInfo.windowApi)
		{
			case WINDOW_API_GLFW:
			{
#ifdef QUARTZAPP_GLFW
				pApplication = CreateGLFWApplication(appInfo);
				break;
#else
				printf("Error creating GLFW Application: GLFW is not available.");
				return nullptr;
#endif
			}

			case WINDOW_API_WINAPI:
			{
#ifdef QUARTZAPP_WINAPI
				pApplication = CreateWinApiApplication(appInfo);
				break;
#else
				printf("Error creating WinApi Application: WinApi is not available.");
				return nullptr;
#endif
			}

			default:
			{
				printf("Error creating Application: Invalid WindowAPI.");
				return nullptr;
			}
		}

		if (!pApplication)
		{
			return nullptr;
		}

		return pApplication;
	}

	void DestroyApplication(Application* pApp)
	{
		switch (pApp->GetWindowAPI())
		{
			case WINDOW_API_GLFW:
			{
#ifdef QUARTZAPP_GLFW
				GLFWApplication* pGLFWApplication = static_cast<GLFWApplication*>(pApp);
				DestroyGLFWApplication(pGLFWApplication);
				break;
#endif
			}

			case WINDOW_API_WINAPI:
			{
#ifdef QUARTZAPP_WINAPI
				WinApiApplication* pWinApiApplication = static_cast<WinApiApplication*>(pApp);
				DestroyWinApiApplication(pWinApiApplication);
				break;
#endif
			}

			default:
			{
				printf("Error destroying Application: Invalid WindowAPI.");
			}
		}
	}
}


