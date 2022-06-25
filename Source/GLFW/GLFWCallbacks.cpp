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

void GLFWWindowKeyCallback(GLFWwindow* pGLFWwindow, int key, int scancode, int action, int mods)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	// HACK: There is unfortunately no easy way to get the glfw 'codepoint' from the scancode
	{
		const char* keyName = glfwGetKeyName(key, scancode);

		if (keyName)
		{
			GLFWHelper::SetKeyRepeating(keyName[0], action & GLFW_REPEAT);
		}
	}

	bool down = action & (GLFW_PRESS | GLFW_REPEAT);

	GLFWHelper::CallWindowKeyCallback(pApp, pWindow, scancode, down, action & GLFW_REPEAT);
}

void GLFWWindowKeyTypedCallback(GLFWwindow* pGLFWwindow, unsigned int codepoint)
{
	using namespace Quartz;

	GLFWWindow* pWindow = (GLFWWindow*)glfwGetWindowUserPointer(pGLFWwindow);
	GLFWApplication* pApp = (GLFWApplication*)pWindow->GetParentApplication();

	// HACK: See GLFWWindowKeyCallback()
	bool repeat = GLFWHelper::IsKeyRepeating((char)codepoint);

	GLFWHelper::CallWindowKeyTypedCallback(pApp, pWindow, (char)codepoint, repeat);
}
