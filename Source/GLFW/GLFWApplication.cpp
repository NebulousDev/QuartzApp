#include "GLFWApplication.h"

#include "Log.h"

#include "GLFWWindow.h"
#include "GLFWRegistry.h"
#include "GLFWHelper.h"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace Quartz
{
	GLFWApplication::GLFWApplication(const ApplicationInfo& appInfo)
		: Application(appInfo) { }

	Window* GLFWApplication::CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo)
	{
		GLFWwindow* pGLFWwindow	= nullptr;
		Surface*	pSurface	= nullptr;
		GLFWWindow* pWindow		= nullptr;

		switch (surfaceInfo.surfaceApi)
		{
			case SURFACE_API_NONE:
			{
				AppLogCallback(mLogCallback, LOG_LEVEL_INFO, "QuartzApp: Creating GLFW Window with no graphics context.");

				break;
			}

			case SURFACE_API_OPENGL:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

				AppLogCallback(mLogCallback, LOG_LEVEL_INFO, "QuartzApp: Creating GLFW Window in OpenGL mode.");

				break;
			} 

			case SURFACE_API_VULKAN:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				AppLogCallback(mLogCallback, LOG_LEVEL_INFO, "QuartzApp: Creating GLFW Window in Vulkan mode.");

				break;
			}

			case SURFACE_API_DX12:
			{
				AppLogCallback(mLogCallback, LOG_LEVEL_ERROR, "QuartzApp: Error creating GLFW Window: DX12 is not available for GLFW windows.");
				return nullptr;
			}

			default:
			{
				AppLogCallback(mLogCallback, LOG_LEVEL_ERROR, "QuartzApp: Error creating GLFW Window: Invalid SurfaceAPI enum.");
				return nullptr;
			}
		}

		pGLFWwindow = glfwCreateWindow(info.width, info.height, (const char*)info.title.Str(), nullptr, nullptr);

		if (!pGLFWwindow)
		{
			GLFWHelper::PrintError(mLogCallback);
			return nullptr;
		}

		switch (surfaceInfo.surfaceApi)
		{
			case SURFACE_API_NONE:
			{
				break;
			}

			case SURFACE_API_OPENGL:
			{
#ifdef QUARTZAPP_GLEW
				pSurface = GLFWHelper::CreateGLFWGLSurface(mLogCallback);
#else
				AppLogCallback(mLogCallback, LOG_LEVEL_ERROR, "QuartzApp: Error creating GLFW GL Window: GLEW is not available.");
				return nullptr;
#endif
				break;
			} 

			case SURFACE_API_VULKAN:
			{

#ifdef QUARTZAPP_VULKAN
				pSurface = GLFWHelper::CreateGLFWVulkanSurface(pGLFWwindow, surfaceInfo, mLogCallback);
#else
				AppLogCallback(mLogCallback, LOG_LEVEL_ERROR, "QuartzApp: Error creating GLFW Vulkan Window: Vulkan is not available.");
				return nullptr;
#endif
				break;
			}
		}

		pWindow = new GLFWWindow(this, pGLFWwindow, info.title, pSurface);

		if (info.hints & WINDOW_FULLSCREEN)
		{
			pWindow->SetFullscreen(true);
		}

		if (info.hints & WINDOW_BORDERLESS)
		{
			pWindow->SetBorderless(true);
		}

		if (info.hints & WINDOW_NO_RESIZE)
		{
			pWindow->SetNoResize(true);
		}

		glfwSetWindowUserPointer(pGLFWwindow, (void*)pWindow);

		glfwSetWindowSizeCallback(pGLFWwindow, GLFWWindowSizeCallback);
		glfwSetWindowPosCallback(pGLFWwindow, GLFWWindowPosCallback);
		glfwSetWindowMaximizeCallback(pGLFWwindow, GLFWWindowMaximizedCallback);
		glfwSetWindowIconifyCallback(pGLFWwindow, GLFWWindowMinimzedCallback);
		glfwSetWindowFocusCallback(pGLFWwindow, GLFWWindowFocusedCallback);
		glfwSetKeyCallback(pGLFWwindow, GLFWKeyCallback);
		glfwSetCharCallback(pGLFWwindow, GLFWKeyTypedCallback);
		glfwSetCursorPosCallback(pGLFWwindow, GLFWMouseMovedCallback);
		glfwSetCursorEnterCallback(pGLFWwindow, GLFWMouseEnteredCallback);

		if (glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(pGLFWwindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		GLFWRegistry::RegisterAppWindow(this, pWindow);
		GLFWHelper::SetWindowState(pWindow, GLFW_WINDOW_STATE_OPEN);

		AppLogCallback(mLogCallback, LOG_LEVEL_INFO, "QuartzApp: GLFW Window (%s) created successfully.", info.title.Str());

		return static_cast<Window*>(pWindow);
	}

	void GLFWApplication::CloseWindow(Window* pWindow)
	{
		GLFWWindow* pGLFWWindow = static_cast<GLFWWindow*>(pWindow);

		if (pGLFWWindow &&
			pGLFWWindow->GetGLFWHandle() &&
			pGLFWWindow->IsOpen())
		{
			GLFWHelper::SetWindowState(pGLFWWindow, GLFW_WINDOW_STATE_CLOSED);
			GLFWHelper::CallWindowClosedCallback(this, pGLFWWindow);

			GLFWRegistry::UnregisterAppWindow(this, pGLFWWindow);

			glfwDestroyWindow(pGLFWWindow->GetGLFWHandle());
		}
	}

	void GLFWApplication::DestroyWindow(Window* pWindow)
	{
		GLFWWindow* pGLFWWindow = static_cast<GLFWWindow*>(pWindow);
		CloseWindow(pGLFWWindow);
		delete pGLFWWindow;
	}

	bool GLFWApplication::IsRawInputAvailable() const
	{
		return false;
	}

	bool GLFWApplication::IsRawInputEnabled() const
	{
		return false;
	}

	void GLFWApplication::UseRawInput(bool useRawInput)
	{
		
	}

	void GLFWApplication::Update()
	{
		// Note that this will be called for every Application which calls Update()
		glfwPollEvents();

		for (GLFWWindow* pWindow : GLFWRegistry::GetWindows(this))
		{
			if (pWindow->IsOpen()) // Dont update closed but non-destroyed windows
			{
				GLFWwindow* pGLFWwindow = pWindow->GetGLFWHandle();

				glfwMakeContextCurrent(pGLFWwindow);

				glfwSwapBuffers(pGLFWwindow);

				if (glfwWindowShouldClose(pGLFWwindow))
				{
					if (pWindow->RequestClose())
					{
						CloseWindow(pWindow);
					}
					else
					{
						// Reset glfwWindowShouldClose()
						glfwSetWindowShouldClose(pGLFWwindow, false);
					}
				}
			}
		}
	}

	void* GLFWApplication::GetNativeHandle()
	{
		// GLFW does not have a native application handle
		return nullptr;
	}

	GLFWApplication* CreateGLFWApplication(const ApplicationInfo& appInfo)
	{
		if (!GLFWHelper::IsGLFWInitialized())
		{
			if (!GLFWHelper::InitializeGLFW(appInfo.logCallback))
			{
				return nullptr;
			}
		}

		GLFWApplication* pApplication = new GLFWApplication(appInfo);

		GLFWRegistry::RegisterApp(pApplication);

		AppLogCallback(appInfo.logCallback, LOG_LEVEL_INFO,
			"QuartzApp: GLFW Application (%s, %s) created succesfully.", appInfo.appName.Str(), appInfo.version.Str());

		return pApplication;
	}

	void DestroyGLFWApplication(GLFWApplication* pGLFWApplication)
	{
		// Intentional copy so as to not remove elements from the itterator live
		Array<GLFWWindow*> appWindows = GLFWRegistry::GetWindows(pGLFWApplication);

		for (GLFWWindow* pWindow : appWindows)
		{
			pGLFWApplication->DestroyWindow(pWindow);
		}

		GLFWRegistry::UnregisterApp(pGLFWApplication);

		if (GLFWRegistry::AppCount() == 0)
		{
			glfwTerminate();
		}

		delete pGLFWApplication;
	}
}

