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

		Point2i			mLastMouse;

	public:
		GLFWWindow(Application* pParentApp, GLFWwindow* pGLFWwindow, 
			const String& title, Surface* pSurface);

		bool RequestClose() override;
		void Close() override;

		bool RecreateSurface(const SurfaceInfo& info) override;

		bool SetTitle(const String& title) override;
		bool Resize(uSize width, uSize height) override;
		bool Resize(const Vec2u size) override;
		bool Move(sSize posX, sSize posY) override;
		bool Move(const Point2i& pos) override;
		bool SetBounds(const Bounds2i& bounds) override;
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
		bool IsMaximized() const override;
		bool IsMinimized() const override;

		bool IsOpen() const override;
		bool IsClosed() const override;
		bool IsCloseRequested() const override;

		bool IsFullscreenAvailable() const override;
		bool IsFullscreen() const override;
		bool SetFullscreen(bool fullscreen) override;

		bool IsBorderlessAvailable() const override;
		bool IsBorderless() const override;
		bool SetBorderless(bool borderless) override;

		bool IsNoResizeAvailable() const override;
		bool IsNoResize() const override;
		bool SetNoResize(bool noResize) override;

		bool IsInvisibleAvailable() const override;
		bool IsInvisible() const override;
		bool SetInvisible(bool invisible) override;

		void* GetNativeHandle() override;

		GLFWwindow* GetGLFWHandle();
	};
}