#include "GLFWWindow.h"

#include "GLFWHelper.h"
#include "GLFWApplication.h"
#include <GLFW/glfw3.h>

#include <cassert>

namespace Quartz
{
	GLFWWindow::GLFWWindow(Application* pParentApp, GLFWwindow* pGLFWwindow, const String& title, Surface* pSurface) :
		Window(pParentApp, pSurface),
		mpGLFWwindow(pGLFWwindow),
		mTitle(title) { }

	bool GLFWWindow::RequestClose()
	{
		GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);
		return GLFWHelper::CallWindowCloseRequestedCallback(pGLFWApp, this);
	}

	void GLFWWindow::Close()
	{
		mpParent->CloseWindow(this);
	}

	bool GLFWWindow::SetTitle(const String& title)
	{
		glfwSetWindowTitle(mpGLFWwindow, title.Str());

		// There is no way to retrieve the window title from GLFW,
		// so we have to store it here.
		mTitle = title;

		return true;
	}

	bool GLFWWindow::Resize(uSize width, uSize height)
	{
		assert(width != 0 && height != 0);

		glfwSetWindowSize(mpGLFWwindow, width, height);

		// GLFW has no way to check the success of glfwSetWindowSize
		return true;
	}

	bool GLFWWindow::Resize(const Vec2u size)
	{
		return Resize(size.x, size.y);
	}

	bool GLFWWindow::Move(uSize posX, uSize posY)
	{
		glfwSetWindowPos(mpGLFWwindow, posX, posY);

		// GLFW has no way to check the success of glfwSetWindowPos
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

		// GLFW has no way to check the success of glfwMaximizeWindow
		return true;
	}

	bool GLFWWindow::Minimize()
	{
		glfwIconifyWindow(mpGLFWwindow);

		// GLFW has no way to check the success of glfwIconifyWindow
		return true;
	}

	String GLFWWindow::GetTitle() const
	{
		return mTitle;
	}

	uSize GLFWWindow::GetWidth() const
	{
		int width;
		glfwGetWindowSize(mpGLFWwindow, &width, nullptr);
		return (uSize)width;
	}

	uSize GLFWWindow::GetHeight() const
	{
		int height;
		glfwGetWindowSize(mpGLFWwindow, nullptr, &height);
		return (uSize)height;
	}

	Vec2i GLFWWindow::GetSize() const
	{
		Vec2i size;
		glfwGetWindowSize(mpGLFWwindow, &size.x, &size.y);
		return size;
	}

	sSize GLFWWindow::GetPosX() const
	{
		int posX;
		glfwGetWindowPos(mpGLFWwindow, &posX, nullptr);
		return (sSize)posX;
	}

	sSize GLFWWindow::GetPosY() const
	{
		int posY;
		glfwGetWindowPos(mpGLFWwindow, nullptr, &posY);
		return (sSize)posY;
	}

	Point2i GLFWWindow::GetPosition() const
	{
		Point2i pos;
		glfwGetWindowPos(mpGLFWwindow, &pos.x, &pos.y);
		return pos;
	}

	Bounds2i GLFWWindow::GetBounds() const
	{
		Point2i pos = GetPosition();
		Vec2i size = GetSize();
		return Bounds2i(pos, pos + size);
	}

	bool GLFWWindow::IsOpen() const
	{
		return mWindowState == GLFW_WINDOW_STATE_OPEN;
	}

	bool GLFWWindow::IsClosed() const
	{
		return mWindowState != GLFW_WINDOW_STATE_OPEN;
	}

	bool GLFWWindow::IsCloseRequested() const
	{
		if (!mpGLFWwindow)
		{
			return true;
		}

		return glfwWindowShouldClose(mpGLFWwindow);
	}

	void* GLFWWindow::GetNativeHandle()
	{
		return mpGLFWwindow;
	}

	GLFWwindow* GLFWWindow::GetGLFWHandle()
	{
		return mpGLFWwindow;
	}
}
