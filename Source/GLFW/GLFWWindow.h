#pragma once

#include "Window.h"

struct GLFWwindow;

namespace Quartz
{
	class GLFWApplication;

	enum GLFWWindowState
	{
		GLFW_WINDOW_STATE_UNINITIALZED,
		GLFW_WINDOW_STATE_CLOSED,
		GLFW_WINDOW_STATE_OPEN
	};


	class GLFWWindow : public Window
	{
	public:
		friend class GLFWHelper;

	private:
		String			mTitle;
		GLFWwindow*		mpGLFWwindow;
		GLFWWindowState mWindowState;

	public:
		GLFWWindow(Application* pParentApp, GLFWwindow* pGLFWwindow, 
			const String& title, Surface* pSurface);

		bool RequestClose() override;
		void Close() override;

		bool SetTitle(const String& title) override;
		bool Resize(uSize width, uSize height) override;
		bool Resize(const Vec2u size) override;
		bool Move(uSize posX, uSize posY) override;
		bool Move(const Point2u& pos) override;
		bool SetBounds(const Bounds2u& bounds) override;
		bool Maximize() override;
		bool Minimize() override;

		String GetTitle() const override;
		uSize GetWidth() const override;
		uSize GetHeight() const override;
		Vec2i GetSize() const override;
		sSize GetPosX() const override;
		sSize GetPosY() const override;
		Point2i GetPosition() const override;
		Bounds2i GetBounds() const override;

		bool IsOpen() const override;
		bool IsClosed() const override;
		bool IsCloseRequested() const override;

		void* GetNativeHandle() override;

		GLFWwindow* GetGLFWHandle();
	};
}