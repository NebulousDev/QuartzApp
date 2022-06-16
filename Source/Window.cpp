#include "Window.h"
#include "Application.h"

namespace Quartz
{
	Window::Window(const WindowInfo& info, Application* pParentApp)
		: mTitle(info.title), 
		mWidth(info.width), 
		mHeight(info.height),
		mPosX(info.posX),
		mPosY(info.posY),
		mpParent(pParentApp) { }

	String Window::GetTitle() const
	{
		return mTitle;
	}

	uSize Window::GetWidth() const
	{
		return mWidth;
	}

	uSize Window::GetHeight() const
	{
		return mHeight;
	}

	Vec2u Window::GetSize() const
	{
		return Vec2u(mWidth, mHeight);
	}

	uSize Window::GetPosX() const
	{
		return mPosX;
	}

	uSize Window::GetPosY() const
	{
		return mPosY;
	}

	Point2u Window::GetPosition() const
	{
		return Point2u(mPosX, mPosY);
	}

	Bounds2u Window::GetBounds() const
	{
		return Bounds2u(GetPosition(), GetSize());
	}

	Application* Window::GetParentApplication()
	{
		return mpParent;
	}

	WindowInfo Window::GetWindowInfo() const
	{
		WindowInfo info;
		info.title	= mTitle;
		info.width	= mWidth;
		info.height	= mHeight;
		info.posX	= mPosX;
		info.posY	= mPosY;

		return info;
	}
}

