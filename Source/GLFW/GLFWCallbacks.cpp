#include "GLFWCallbacks.h"

#include "GLFWHelper.h"
#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowSizeCallback(pApp, pWindow, width, height);
}

void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowPosCallback(pApp, pWindow, posX, posY);
}

void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowClosedCallback(pApp, pWindow);
}

bool GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	return GLFWHelper::CallWindowCloseRequestedCallback(pApp, pWindow);
}

void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int maximized)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowMaximizedCallback(pApp, pWindow, maximized);
}

void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowMinimizedCallback(pApp, pWindow, minimized);
}

void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	GLFWHelper::CallWindowFocusedCallback(pApp, pWindow, focused);
}