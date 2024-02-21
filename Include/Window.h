#pragma once

#include "DLL.h"
#include "Types/Types.h"
#include "Types/String.h"
#include "Math/Bounds.h"
#include "Surface.h"

namespace Quartz
{
	class Application;

	enum WindowHint
	{
		WINDOW_WINDOWED		= 0x01,
		WINDOW_FULLSCREEN	= 0x02,
		WINDOW_BORDERLESS	= 0x04,
		WINDOW_NO_RESIZE	= 0x08,
		WINDOW_INVISIBLE	= 0x10
	};

	typedef flags32 WindowHints;

	struct WindowInfo
	{
		String		title;
		uSize		width;
		uSize		height;
		uSize		posX;
		uSize		posY;
		WindowHints	hints;
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

		virtual bool RecreateSurface(const SurfaceInfo& info) = 0;

		virtual bool SetTitle(const String& title) = 0;
		virtual bool Resize(uSize width, uSize height) = 0;
		virtual bool Resize(const Vec2u size) = 0;
		virtual bool Move(sSize posX, sSize posY) = 0;
		virtual bool Move(const Point2i& pos) = 0;
		virtual bool SetBounds(const Bounds2i& bounds) = 0;
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
		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;

		virtual bool IsOpen() const = 0;
		virtual bool IsClosed() const = 0;
		virtual bool IsCloseRequested() const = 0;

		// Note: if your graphics api allows exclusive fullscreen
		//		 it should be used instead of these functions
		virtual bool IsFullscreenAvailable() const = 0;
		virtual bool IsFullscreen() const = 0;
		virtual bool SetFullscreen(bool fullscreen) = 0;

		virtual bool IsBorderlessAvailable() const = 0;
		virtual bool IsBorderless() const = 0;
		virtual bool SetBorderless(bool borderless) = 0;

		virtual bool IsNoResizeAvailable() const = 0;
		virtual bool IsNoResize() const = 0;
		virtual bool SetNoResize(bool noResize) = 0;

		virtual bool IsInvisibleAvailable() const = 0;
		virtual bool IsInvisible() const = 0;
		virtual bool SetInvisible(bool invisible) = 0;

		virtual void* GetNativeHandle() const = 0;

		Application* GetParentApplication();
		Surface* GetSurface();

		WindowInfo GetWindowInfo() const;
	};
}