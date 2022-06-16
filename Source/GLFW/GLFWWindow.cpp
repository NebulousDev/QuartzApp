#include "GLFWWindow.h"

#include "GLFWHelper.h"
#include "Application.h"
#include <GLFW/glfw3.h>

#include <cassert>

namespace Quartz
{
	GLFWWindow::GLFWWindow(const WindowInfo& info, Application* pParentApp)
		: Window(info, pParentApp), 
		mpGLFWwindow(nullptr) { }

	bool GLFWWindow::Create()
	{
		mpGLFWwindow = glfwCreateWindow(mWidth, mHeight, (const char*)mTitle.Str(), NULL, NULL);

		if (!mpGLFWwindow)
		{
			GLFWHelper::PrintError();
			return false;
		}

		glfwSetWindowUserPointer(mpGLFWwindow, (void*)this);

		GLFWHelper::RegisterAppWindow(reinterpret_cast<GLFWApplication*>(mpParent), this);

		return true;
	}

	void GLFWWindow::Destroy()
	{
		if (mpGLFWwindow)
		{
			glfwDestroyWindow(mpGLFWwindow);
			GLFWHelper::UnregisterAppWindow(reinterpret_cast<GLFWApplication*>(mpParent), this);
		}
	}

	bool GLFWWindow::SetTitle(const String& title)
	{
		glfwSetWindowTitle(mpGLFWwindow, title.Str());

		// There is no way to retrieve the window title from GLFW,
		// so we have to trust that glfwSetWindowTitle worked.
		mTitle = title;

		return true;
	}

	bool GLFWWindow::Resize(uSize width, uSize height)
	{
		assert(width != 0 && height != 0);

		glfwSetWindowSize(mpGLFWwindow, width, height);

		// Note, returned width and hight may be zero if an error occured
		int newWidth, newHeight;
		glfwGetWindowSize(mpGLFWwindow, &newWidth, &newHeight);

		if (newWidth == 0 || newHeight == 0)
		{
			return false;
		}

		mWidth = newWidth;
		mHeight = newHeight;

		return true;
	}

	bool GLFWWindow::Resize(const Vec2u size)
	{
		return Resize(size.x, size.y);
	}

	bool GLFWWindow::Move(uSize posX, uSize posY)
	{
		glfwSetWindowPos(mpGLFWwindow, posX, posY);

		// Note, returned x and y may be zero if an error occured
		int newX, newY;
		glfwGetWindowPos(mpGLFWwindow, &newX, &newY);

		mWidth = newX;
		mHeight = newY;

		return true; 
	}

	bool GLFWWindow::Move(const Point2u& pos)
	{
		return Move(pos.x, pos.y);
	}

	bool GLFWWindow::SetBounds(const Bounds2u& bounds)
	{
		return 
			Resize(bounds.Extent()) &&
			Move(bounds.BottomLeft());
	}

	bool GLFWWindow::Maximize()
	{
		glfwMaximizeWindow(mpGLFWwindow);

		// GLFW has no way to confirm if Maximize succeeded
		return true;
	}

	bool GLFWWindow::Minimize()
	{
		glfwIconifyWindow(mpGLFWwindow);

		// GLFW has no way to confirm if Minimize(Iconify) succeeded
		return true;
	}

	bool GLFWWindow::CloseRequested()
	{
		if (!mpGLFWwindow)
		{
			return true;
		}

		return glfwWindowShouldClose(mpGLFWwindow);
	}

}
