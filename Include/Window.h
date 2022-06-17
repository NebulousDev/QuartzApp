#pragma once

#include "DLL.h"
#include "Types/Types.h"
#include "Types/String.h"
#include "Math/Bounds.h"
#include "Surface.h"

namespace Quartz
{
	class Application;

	struct WindowInfo
	{
		String		title;
		uSize		width;
		uSize		height;
		uSize		posX;
		uSize		posY;
	};

	class QUARTZAPP_API Window
	{
	protected:
		Application*	mpParent;
		Surface*		mpSurface;

	public:
		Window(Application* pParentApp, Surface* pSurface);

		virtual bool RequestClose() = 0;
		virtual void Close() = 0;

		virtual bool SetTitle(const String& title) = 0;
		virtual bool Resize(uSize width, uSize height) = 0;
		virtual bool Resize(const Vec2u size) = 0;
		virtual bool Move(uSize posX, uSize posY) = 0;
		virtual bool Move(const Point2u& pos) = 0;
		virtual bool SetBounds(const Bounds2u& bounds) = 0;
		virtual bool Maximize() = 0;
		virtual bool Minimize() = 0;

		virtual String GetTitle() const = 0;
		virtual uSize GetWidth() const = 0;
		virtual uSize GetHeight() const = 0;
		virtual Vec2i GetSize() const = 0;
		virtual sSize GetPosX() const = 0;
		virtual sSize GetPosY() const = 0;
		virtual Point2i GetPosition() const = 0;
		virtual Bounds2i GetBounds() const = 0;

		virtual bool IsOpen() const = 0;
		virtual bool IsClosed() const = 0;
		virtual bool IsCloseRequested() const = 0;

		virtual void* GetNativeHandle() = 0;

		Application* GetParentApplication();
		Surface* GetSurface();

		WindowInfo GetWindowInfo() const;
	};
}