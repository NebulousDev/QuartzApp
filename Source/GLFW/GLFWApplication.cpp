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
			DestroyWindow(pWindow);
		}

		GLFWHelper::UnregisterApp(this);

		if (GLFWHelper::AppCount() == 0)
		{
			glfwTerminate();
		}
	}

	Window* GLFWApplication::CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo)
	{
		GLFWwindow* pGLFWwindow	= nullptr;
		Surface*	pSurface	= nullptr;
		GLFWWindow* pWindow		= nullptr;

		switch (surfaceInfo.surfaceApi)
		{
			case SURFACE_API_OPENGL:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

				printf("Creating GLFW Window in OpenGL mode.\n");

				break;
			} 

			case SURFACE_API_VULKAN:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				printf("Creating GLFW Window in Vulkan mode.\n");

				break;
			}

			case SURFACE_API_DX12:
			{
				printf("Error creating GLFW Window: DX12 is not available for GLFW windows.");
				return false;
			}

			default:
			{
				printf("Error creating GLFW Window: Invalid SurfaceAPI enum.");
				return false;
			}
		}

		pGLFWwindow = glfwCreateWindow(info.width, info.height, (const char*)info.title.Str(), nullptr, nullptr);

		if (!pGLFWwindow)
		{
			GLFWHelper::PrintError();
			return false;
		}

		switch (surfaceInfo.surfaceApi)
		{
			case SURFACE_API_OPENGL:
			{
#ifdef QUARTZAPP_GLEW
				pSurface = GLFWHelper::CreateGLFWGLSurface();
#else
				printf("Error creating GLFW Vulkan Window: Vulkan is not available.");
				return false;
#endif
				if (!pSurface)
				{
					return false;
				}

				break;
			} 

			case SURFACE_API_VULKAN:
			{

#ifdef QUARTZAPP_VULKAN
				pSurface = GLFWHelper::CreateGLFWVulkanSurface(pGLFWwindow, surfaceInfo);
#else
				printf("Error creating GLFW GL Window: GLEW is not available.");
				return false;
#endif
				if (!pSurface)
				{
					return false;
				}

				break;
			}
		}

		pWindow = new GLFWWindow(this, pGLFWwindow, info.title, pSurface);

		glfwSetWindowUserPointer(pGLFWwindow, (void*)pWindow);

		GLFWHelper::RegisterAppWindow(this, pWindow);
		GLFWHelper::SetWindowState(pWindow, GLFW_WINDOW_STATE_OPEN);

		return static_cast<Window*>(pWindow);
	}

	void GLFWApplication::CloseWindow(Window* pWindow)
	{
		GLFWWindow* pGLFWWindow = static_cast<GLFWWindow*>(pWindow);

		if (pGLFWWindow &&
			pGLFWWindow->GetGLFWHandle() &&
			pGLFWWindow->IsOpen())
		{
			GLFWHelper::CallWindowClosedCallback(this, pGLFWWindow);
			GLFWHelper::SetWindowState(pGLFWWindow, GLFW_WINDOW_STATE_CLOSED);

			glfwDestroyWindow(pGLFWWindow->GetGLFWHandle());
		}
	}

	void GLFWApplication::DestroyWindow(Window* pWindow)
	{
		GLFWWindow* pGLFWWindow = static_cast<GLFWWindow*>(pWindow);

		CloseWindow(pWindow);
		GLFWHelper::UnregisterAppWindow(this, pGLFWWindow);

		delete pGLFWWindow;
	}

	void GLFWApplication::Update()
	{
		// Note that this will be called for every Application which calls Update()
		glfwPollEvents();

		for (GLFWWindow* pWindow : GLFWHelper::GetWindows(this))
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
}

