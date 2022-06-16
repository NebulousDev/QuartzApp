#pragma once

#include "Window.h"

struct GLFWwindow;

namespace Quartz
{
	class GLFWApplication;

	class GLFWWindow : public Window
	{
	public:
		friend class GLFWHelper;

	private:
		GLFWwindow*	mpGLFWwindow;

	public:
		GLFWWindow(const WindowInfo& info, Application* pParentApp);

		bool Create() override;
		void Destroy() override;

		bool SetTitle(const String& title) override;
		bool Resize(uSize width, uSize height) override;
		bool Resize(const Vec2u size) override;
		bool Move(uSize posX, uSize posY) override;
		bool Move(const Point2u& pos) override;
		bool SetBounds(const Bounds2u& bounds) override;
		bool Maximize() override;
		bool Minimize() override;

		bool CloseRequested() override;
	};
}