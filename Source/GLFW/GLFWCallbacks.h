#pragma once

struct GLFWwindow;

void GLFWWindowSizeCallback(GLFWwindow* pGLFWwindow, int width, int height);
void GLFWWindowPosCallback(GLFWwindow* pGLFWwindow, int posX, int posY);
void GLFWWindowClosedCallback(GLFWwindow* pGLFWwindow);
bool GLFWWindowCloseRequestedCallback(GLFWwindow* pGLFWwindow);
void GLFWWindowMaximizedCallback(GLFWwindow* pGLFWwindow, int maximized);
void GLFWWindowMinimzedCallback(GLFWwindow* pGLFWwindow, int minimized);
void GLFWWindowFocusedCallback(GLFWwindow* pGLFWwindow, int focused);
void GLFWKeyCallback(GLFWwindow* pGLFWwindow, int key, int scancode, int action, int mods);
void GLFWKeyTypedCallback(GLFWwindow* pGLFWwindow, unsigned int codepoint);
void GLFWMouseMovedCallback(GLFWwindow* pGLFWwindow, double mouseX, double mouseY);
void GLFWMouseMovedRelativeCallback(GLFWwindow* pGLFWwindow, double relX, double relY);
void GLFWMouseEnteredCallback(GLFWwindow* pGLFWwindow, int entered);