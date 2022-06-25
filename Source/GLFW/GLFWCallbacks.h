#pragma once

struct GLFWwindow;

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY);
void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
bool GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow);
void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int maximized);
void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized);
void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused);
void GLFWWindowKeyCallback(GLFWwindow* pGLFWwindow, int key, int scancode, int action, int mods);
void GLFWWindowKeyTypedCallback(GLFWwindow* pGLFWwindow, unsigned int codepoint);