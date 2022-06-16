#pragma once

#include "Types/Types.h"
#include "Types/String.h"
#include "Math/Bounds.h"

namespace Quartz
{
	class Application;

	enum WindowState
	{
		WINDOW_STATE_UNINITIALZED,
		WINDOW_STATE_CLOSED,
		WINDOW_STATE_OPEN
	};

	struct WindowInfo
	{
		String		title;
		uSize		width;
		uSize		height;
		uSize		posX;
		uSize		posY;
	};

	class Window
	{
	protected:
		String		mTitle;
		uSize		mWidth;
		uSize		mHeight;
		uSize		mPosX;
		uSize		mPosY;

		Application* mpParent;

	public:
		Window(const WindowInfo& info, Application* pParentApp);

		virtual bool Create() = 0;
		virtual void Destroy() = 0;

		virtual bool SetTitle(const String& title) = 0;
		virtual bool Resize(uSize width, uSize height) = 0;
		virtual bool Resize(const Vec2u size) = 0;
		virtual bool Move(uSize posX, uSize posY) = 0;
		virtual bool Move(const Point2u& pos) = 0;
		virtual bool SetBounds(const Bounds2u& bounds) = 0;
		virtual bool Maximize() = 0;
		virtual bool Minimize() = 0;

		String GetTitle() const;
		uSize GetWidth() const;
		uSize GetHeight() const;
		Vec2u GetSize() const;
		uSize GetPosX() const;
		uSize GetPosY() const;
		Point2u GetPosition() const;
		Bounds2u GetBounds() const;

		virtual bool CloseRequested() = 0;

		Application* GetParentApplication();
		WindowInfo GetWindowInfo() const;
	};
}