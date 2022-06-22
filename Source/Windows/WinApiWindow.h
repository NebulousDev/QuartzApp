#pragma once

#include "Window.h"
#include "WinApi.h"

namespace Quartz
{
	class WinApiWindow : public Window
	{
	public:
		friend class WinApiHelper;

	private:
		HWND	mHwnd;
		bool	mOpen;
		bool	mCloseRequested;
		bool	mFullscreen;
		DWORD	mRestoreStyle;

	public:
		WinApiWindow(Application* pParentApp, Surface* pSurface, DWORD dwRestoreStyle, HWND hwnd);

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

		bool SetStyle(DWORD dwStyle);
		void SetRestoreStyle(DWORD dwStyle);
		bool RestoreStyle();

		void* GetNativeHandle() override;

		HWND GetHWND();
	};
}