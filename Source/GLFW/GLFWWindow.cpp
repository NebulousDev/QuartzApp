#include "GLFWWindow.h"

#include "GLFWHelper.h"
#include "GLFWApplication.h"
#include <GLFW/glfw3.h>

#include "Log.h"

#include <cassert>

namespace Quartz
{
	GLFWWindow::GLFWWindow(Application* pParentApp, GLFWwindow* pGLFWwindow, const String& title, Surface* pSurface) :
		Window(pParentApp, pSurface),
		mpGLFWwindow(pGLFWwindow),
		mTitle(title),
		mWindowState(GLFW_WINDOW_STATE_UNINITIALZED) { }

	bool GLFWWindow::RequestClose()
	{
		GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);
		return GLFWHelper::CallWindowCloseRequestedCallback(pGLFWApp, this);
	}

	void GLFWWindow::Close()
	{
		mpParent->CloseWindow(this);
	}

	bool GLFWWindow::RecreateSurface(const SurfaceInfo& info)
	{
		return true;
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

	bool GLFWWindow::Move(sSize posX, sSize posY)
	{
		glfwSetWindowPos(mpGLFWwindow, posX, posY);

		// GLFW has no way to check the success of glfwSetWindowPos
		return true; 
	}

	bool GLFWWindow::Move(const Point2i& pos)
	{
		return Move(pos.x, pos.y);
	}

	bool GLFWWindow::SetBounds(const Bounds2i& bounds)
	{
		Vec2i   size = bounds.Extent();
		Point2i pos  = bounds.BottomLeft();

		glfwSetWindowSize(mpGLFWwindow, size.x, size.y);
		glfwSetWindowPos(mpGLFWwindow, pos.x, pos.y);

		// GLFW has no way to check success
		return true;
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

	bool GLFWWindow::IsMaximized() const
	{
		return glfwGetWindowAttrib(mpGLFWwindow, GLFW_MAXIMIZED);
	}

	bool GLFWWindow::IsMinimized() const
	{
		return glfwGetWindowAttrib(mpGLFWwindow, GLFW_ICONIFIED);
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

	bool GLFWWindow::IsFullscreenAvailable() const
	{
		return true;
	}

	bool GLFWWindow::IsFullscreen() const
	{
		return glfwGetWindowMonitor(mpGLFWwindow) == nullptr;
	}

	bool GLFWWindow::SetFullscreen(bool fullscreen)
	{
		if (fullscreen == (glfwGetWindowMonitor(mpGLFWwindow) != nullptr))
		{
			return true;
		}

		GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);

		if (fullscreen)
		{
			GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
			Vec2i size = GetSize();

			glfwSetWindowMonitor(mpGLFWwindow, pMonitor, 0, 0, size.x, size.y, GLFW_DONT_CARE); // TODO: custom refresh rate

			if (glfwGetWindowMonitor(mpGLFWwindow) == nullptr)
			{
				AppLogCallback(pGLFWApp->GetLogCallback(), LOG_LEVEL_ERROR, 
					"QuartzApp: Error setting window fullscreen: glfwSetWindowMonitor() failed.");

				return false;
			}
		}
		else
		{
			Point2i pos = GetPosition();
			Vec2i size  = GetSize();

			glfwSetWindowMonitor(mpGLFWwindow, nullptr, pos.x, pos.y, size.x, size.y, 0);
		}

		AppLogCallback(pGLFWApp->GetLogCallback(), LOG_LEVEL_INFO,
			"QuartzApp: Set window (%s) fullscreen (%s).", GetTitle().Str(), fullscreen ? "true" : "false");

		return true;
	}

	bool GLFWWindow::IsBorderlessAvailable() const
	{
		return true;
	}

	bool GLFWWindow::IsBorderless() const
	{
		return glfwGetWindowAttrib(mpGLFWwindow, GLFW_DECORATED) == GLFW_FALSE;
	}

	bool GLFWWindow::SetBorderless(bool borderless)
	{
		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_DECORATED) != (int)borderless)
		{
			return true;
		}

		int glfwBool = borderless ? GLFW_FALSE : GLFW_TRUE; // Note: Inverted

		glfwSetWindowAttrib(mpGLFWwindow, GLFW_DECORATED, glfwBool);

		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_DECORATED) != glfwBool)
		{
			GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);
			AppLogCallback(pGLFWApp->GetLogCallback(), LOG_LEVEL_ERROR,
				"QuartzApp: Error setting window borderless: glfwSetWindowAttrib() failed.");
			return false;
		}

		return true;
	}

	bool GLFWWindow::IsNoResizeAvailable() const
	{
		return true;
	}

	bool GLFWWindow::IsNoResize() const
	{
		return glfwGetWindowAttrib(mpGLFWwindow, GLFW_RESIZABLE) == GLFW_FALSE;
	}

	bool GLFWWindow::SetNoResize(bool noResize)
	{
		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_RESIZABLE) == (int)noResize)
		{
			return true;
		}

		int glfwBool = noResize ? GLFW_FALSE : GLFW_TRUE; // Note: Inverted

		glfwSetWindowAttrib(mpGLFWwindow, GLFW_RESIZABLE, glfwBool);

		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_RESIZABLE) != glfwBool)
		{
			GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);
			AppLogCallback(pGLFWApp->GetLogCallback(), LOG_LEVEL_ERROR,
				"QuartzApp: Error setting window unresizable: glfwSetWindowAttrib() failed.");
			return false;
		}

		return true;
	}

	bool GLFWWindow::IsInvisibleAvailable() const
	{
		return true;
	}

	bool GLFWWindow::IsInvisible() const
	{
		return glfwGetWindowAttrib(mpGLFWwindow, GLFW_VISIBLE);
	}

	bool GLFWWindow::SetInvisible(bool invisible)
	{
		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_VISIBLE) == (int)invisible)
		{
			return true;
		}

		int glfwBool = invisible ? GLFW_FALSE : GLFW_TRUE; // Note: Inverted

		glfwSetWindowAttrib(mpGLFWwindow, GLFW_VISIBLE, glfwBool);

		if (glfwGetWindowAttrib(mpGLFWwindow, GLFW_VISIBLE) != glfwBool)
		{
			GLFWApplication* pGLFWApp = static_cast<GLFWApplication*>(mpParent);
			AppLogCallback(pGLFWApp->GetLogCallback(), LOG_LEVEL_ERROR,
				"QuartzApp: Error setting window visibility: glfwSetWindowAttrib() failed.");
			return false;
		}

		return true;
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
