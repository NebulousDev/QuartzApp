#include "Window.h"
#include "Application.h"

namespace Quartz
{
	Window::Window(Application* pParentApp, Surface* pSurface) :
		mpParent(pParentApp), 
		mpSurface(pSurface) { }

	Surface* Window::GetSurface()
	{
		return mpSurface;
	}

	Application* Window::GetParentApplication()
	{
		return mpParent;
	}

	WindowInfo Window::GetWindowInfo() const
	{
		Point2i pos = GetPosition();
		Vec2i size = GetSize();

		WindowInfo info;
		info.title	= GetTitle();
		info.width	= size.x;
		info.height	= size.y;
		info.posX	= pos.x;
		info.posY	= pos.y;

		return info;
	}
}

