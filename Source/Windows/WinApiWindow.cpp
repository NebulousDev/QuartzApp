#include "WinApiWindow.h"

#include "WinApiApplication.h"
#include "WinApiHelper.h"

namespace Quartz
{
	WinApiWindow::WinApiWindow(Application* pParentApp, Surface* pSurface, const WindowStyle& restoreStyle, HWND hwnd) :
		Window(pParentApp, pSurface),
		mHwnd(hwnd),
		mOpen(true),
		mCloseRequested(false),
		mFullscreen(false),
		mLastMinimized(false),
		mLastMaximized(false),
		mRestoreStyle(restoreStyle),
		mMouseInside(false) { }

	bool WinApiWindow::RequestClose()
	{
		WinApiApplication* pWinApiApp = static_cast<WinApiApplication*>(mpParent);

		if (WinApiHelper::CallWindowCloseRequestedCallback(pWinApiApp, this))
		{
			PostMessageA(mHwnd, WM_CLOSE, 0, WINAPI_CLOSE_REQUEST_PARAM);
			return true;
		}

		return false;
	}

	void WinApiWindow::Close()
	{
		// TODO: Implment
	}

	bool WinApiWindow::RecreateSurface(const SurfaceInfo& info)
	{
		WinApiApplication* pWinApiApp = static_cast<WinApiApplication*>(mpParent);
		HINSTANCE hInstance = pWinApiApp->GetInstance();

		Surface* pNewSurface = WinApiHelper::CreateSurface(hInstance, mHwnd, info, pWinApiApp->GetLogCallback());

		if (!pNewSurface && (info.surfaceApi != SURFACE_API_NONE))
		{
			AppLogCallback(pWinApiApp->GetLogCallback(), LOG_LEVEL_ERROR, 
				"QuartzApp: Error creating new Surface: CreateSurface() failed. Using existing...\n");

			return false;
		}

		delete mpSurface; // TODO: Add more logic here

		mpSurface = pNewSurface;
	}

	bool WinApiWindow::SetTitle(const String& title)
	{
		BOOL result = SetWindowText(mHwnd, title.Str());

		if (result == 0)
		{
			return false;
		}

		return true;
	}

	bool WinApiWindow::Resize(uSize width, uSize height)
	{
		DWORD dwStyle = (DWORD)GetWindowLong(mHwnd, GWL_STYLE);

		RECT windowSize = { 0, 0, width, height };
		AdjustWindowRect(&windowSize, dwStyle, FALSE);

		BOOL result = SetWindowPos(
			mHwnd, HWND_TOP, 0, 0,
			windowSize.right - windowSize.left, 
			windowSize.bottom - windowSize.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		);

		if (result == 0)
		{
			return false;
		}

		return true;
	}
	
	bool WinApiWindow::Resize(const Vec2u size)
	{
		return Resize(size.x, size.y);
	}

	bool WinApiWindow::Move(sSize posX, sSize posY)
	{
		DWORD dwStyle = (DWORD)GetWindowLong(mHwnd, GWL_STYLE);

		RECT windowSize = { posX, posY, 0, 0 };
		AdjustWindowRect(&windowSize, dwStyle, FALSE);

		BOOL result = SetWindowPos(
			mHwnd, HWND_TOP, 
			windowSize.left, windowSize.top, 0, 0, 
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
		);

		if (result == 0)
		{
			return false;
		}

		return true;
	}

	bool WinApiWindow::Move(const Point2i& pos)
	{
		return Move(pos.x, pos.y);
	}

	bool WinApiWindow::SetBounds(const Bounds2i& bounds)
	{
		Vec2i   size = bounds.Extent();
		Point2i pos  = bounds.BottomLeft();

		DWORD dwStyle = (DWORD)GetWindowLong(mHwnd, GWL_STYLE);

		RECT windowSize = { pos.x, pos.y, size.x, size.y };
		AdjustWindowRect(&windowSize, dwStyle, FALSE);

		BOOL result = SetWindowPos(
			mHwnd, HWND_TOP,
			windowSize.left, windowSize.top,
			windowSize.right - windowSize.left, 
			windowSize.bottom - windowSize.top,
			SWP_NOZORDER | SWP_NOACTIVATE
		);

		if (result == 0)
		{
			return false;
		}

		return true;
	}

	bool WinApiWindow::Maximize()
	{
		return ShowWindow(mHwnd, SW_MAXIMIZE);
	}

	bool WinApiWindow::Minimize()
	{	
		return ShowWindow(mHwnd, SW_MINIMIZE);
	}

	String WinApiWindow::GetTitle() const
	{
		char title[128] {};
		GetWindowTextA(mHwnd, title, 127);
		return String(title);
	}

	uSize WinApiWindow::GetWidth() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);
		return rect.right - rect.left;
	}

	uSize WinApiWindow::GetHeight() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);
		return rect.bottom - rect.top;
	}

	Vec2i WinApiWindow::GetSize() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);

		return Vec2i
		(
			rect.right - rect.left, 
			rect.bottom - rect.top
		);
	}

	sSize WinApiWindow::GetPosX() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);

		return rect.left;
	}

	sSize WinApiWindow::GetPosY() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);

		return rect.top;
	}

	Point2i WinApiWindow::GetPosition() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);

		return Point2i(rect.left, rect.top);
	}

	Bounds2i WinApiWindow::GetBounds() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);

		return Bounds2i
		(
			{ rect.left, rect.bottom }, 
			{ rect.right, rect.top }
		);
	}

	bool WinApiWindow::IsMaximized() const
	{
		WINDOWPLACEMENT placement;
		GetWindowPlacement(mHwnd, &placement);
		return placement.showCmd & SW_MAXIMIZE;
	}

	bool WinApiWindow::IsMinimized() const
	{
		WINDOWPLACEMENT placement;
		GetWindowPlacement(mHwnd, &placement);
		return placement.showCmd & SW_MINIMIZE;
	}

	bool WinApiWindow::IsOpen() const
	{
		return mOpen;
	}

	bool WinApiWindow::IsClosed() const
	{
		return !mOpen;
	}

	bool WinApiWindow::IsCloseRequested() const
	{
		return mCloseRequested;
	}

	bool WinApiWindow::IsFullscreenAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsFullscreen() const
	{
		return mFullscreen;
	}

	bool WinApiWindow::SetFullscreen(bool fullscreen)
	{
		if(fullscreen == mFullscreen)
		{
			return true;
		}

		WinApiApplication* pWinApiApp = static_cast<WinApiApplication*>(mpParent);

		if (!mFullscreen)
		{
			Vec2i size = GetSize();
			uSize refreshRate = WinApiHelper::GetDefaultMonitorRefreshRate();

			if (!WinApiHelper::SetDisplayMode(0, size.x, size.y, refreshRate, pWinApiApp->GetLogCallback()))
			{
				return false;
			}

			mRestorePos = GetPosition();
			Move(0, 0); // Error check?

			mFullscreen = true;
		}
		else
		{
			Vec2u size = WinApiHelper::GetDefaultMonitorSize();
			uSize refreshRate = WinApiHelper::GetDefaultMonitorRefreshRate();

			if (!WinApiHelper::SetDisplayMode(0, size.x, size.y, refreshRate, pWinApiApp->GetLogCallback()))
			{
				return false;
			}

			Move(mRestorePos); // Error check?

			mFullscreen = false;
		}

		AppLogCallback(pWinApiApp->GetLogCallback(), LOG_LEVEL_INFO,
			"QuartzApp: Set window fullscreen (%s).", fullscreen ? "true" : "false");

		return true;
	}

	bool WinApiWindow::IsBorderlessAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsBorderless() const
	{
		DWORD dwStyle = GetDWORDStyle();
		return dwStyle & WS_POPUPWINDOW;
	}

	bool WinApiWindow::SetBorderless(bool borderless)
	{
		DWORD dwStyle = GetDWORDStyle();

		if (borderless)
		{
			dwStyle &= ~WS_OVERLAPPEDWINDOW;
			dwStyle |= WS_POPUPWINDOW;

			mRestoreStyle.borderless = true;

			if (!SetDWORDStyle(dwStyle))
			{
				mRestoreStyle.borderless = false;
				return false;
			}

			return true;
		}
		else
		{
			mRestoreStyle.borderless = false;
			return RestoreStyle();
		}
	}

	bool WinApiWindow::IsNoResizeAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsNoResize() const
	{
		DWORD dwStyle = GetDWORDStyle();
		return !(dwStyle & WS_THICKFRAME);
	}

	bool WinApiWindow::SetNoResize(bool noResize)
	{
		DWORD dwStyle = GetDWORDStyle();

		if (noResize)
		{
			dwStyle &= ~WS_THICKFRAME;
			mRestoreStyle.noResize = true;
		}
		else
		{
			dwStyle |= WS_THICKFRAME;
			mRestoreStyle.noResize = false;
		}

		if (!SetDWORDStyle(dwStyle))
		{
			mRestoreStyle.noResize = false;
			return false;
		}

		return true;
	}

	bool WinApiWindow::IsInvisibleAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsInvisible() const
	{
		DWORD dwStyle = GetDWORDStyle();
		return ~(dwStyle & WS_VISIBLE);
	}

	bool WinApiWindow::SetInvisible(bool invisible)
	{
		DWORD dwStyle = GetDWORDStyle();

		if (invisible)
		{
			dwStyle &= ~WS_VISIBLE;
			mRestoreStyle.invisible = true;
		}
		else
		{
			dwStyle |= WS_VISIBLE;
			mRestoreStyle.invisible = false;
		}

		if (!SetDWORDStyle(dwStyle))
		{
			mRestoreStyle.invisible = false;
			return false;
		}

		return true;
	}

	DWORD WinApiWindow::GetDWORDStyle() const
	{
		return GetWindowLongA(mHwnd, GWL_STYLE);
	}

	DWORD WinApiWindow::GetDWORDStyle(const WindowStyle& state) const
	{
		DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

		if (state.borderless)
		{
			dwStyle = WS_POPUPWINDOW | WS_VISIBLE;
		}
		if (state.noResize)
		{
			dwStyle &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		}
		if (state.invisible)
		{
			dwStyle &= ~WS_VISIBLE;
		}

		return dwStyle;
	}

	bool WinApiWindow::SetDWORDStyle(DWORD dwStyle)
	{
		SetWindowLongA(mHwnd, GWL_STYLE, dwStyle);

		if (!SetWindowPos(
			mHwnd, 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE |
			SWP_NOZORDER | SWP_FRAMECHANGED))
		{
			return false;
		}

		return true;
	}

	bool WinApiWindow::SetStyle(const WindowStyle& style)
	{
		DWORD dwStyle = GetDWORDStyle(style);
		return SetDWORDStyle(dwStyle);
	}

	void WinApiWindow::SetRestoreStyle(const WindowStyle& state)
	{
		mRestoreStyle = state;
	}

	bool WinApiWindow::RestoreStyle()
	{
		return SetStyle(mRestoreStyle);
	}

	WindowStyle WinApiWindow::GetRestoreStyle() const
	{
		return mRestoreStyle;
	}

	Point2i WinApiWindow::GetRestorePos() const
	{
		return mRestorePos;
	}

	void* WinApiWindow::GetNativeHandle()
	{
		return (void*)mHwnd;
	}

	HWND WinApiWindow::GetHWND()
	{
		return mHwnd;
	}
}

