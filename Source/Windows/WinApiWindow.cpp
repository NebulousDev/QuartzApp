#include "WinApiWindow.h"

#include "WinApiApplication.h"
#include "WinApiHelper.h"

namespace Quartz
{
	WinApiWindow::WinApiWindow(Application* pParentApp, Surface* pSurface, DWORD dwRestoreStyle, HWND hwnd) :
		Window(pParentApp, pSurface),
		mHwnd(hwnd),
		mOpen(false),
		mRestoreStyle(dwRestoreStyle) { }

	bool WinApiWindow::RequestClose()
	{
		// TODO: Implement
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

		if (mpSurface)
		{
			delete mpSurface;
		}

		switch (info.surfaceApi)
		{
			case SURFACE_API_NONE:
			{
				mpSurface = nullptr;
				return true;
			}

			case SURFACE_API_OPENGL:
			{
#ifdef QUARTZAPP_GLEW
				mpSurface = WinApiHelper::CreateWinApiGLFWGLSurface();
#else
				printf("Error creating Windows GL Surface: GLEW is not available.");
				return false;
#endif
				return true;
			}

			case SURFACE_API_VULKAN:
			{

#ifdef QUARTZAPP_VULKAN
				mpSurface = WinApiHelper::CreateWinApiVulkanSurface(hInstance, mHwnd, info);
#else
				printf("Error creating Windows Vulkan Surface: Vulkan is not available.");
				return false;
#endif
				return true;
			}
		}
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
		return rect.top - rect.bottom;
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
		Vec2i size = GetSize();
		return WinApiHelper::SetDisplayMode(0, size.x, size.y, 165);
	}

	bool WinApiWindow::IsBorderlessAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsBorderless() const
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);
		return dwStyle & WS_POPUPWINDOW;
	}

	bool WinApiWindow::SetBorderless(bool borderless)
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);

		if (borderless == (dwStyle & WS_POPUPWINDOW))
		{
			return true;
		}

		if (borderless)
		{
			SetRestoreStyle(dwStyle);
			
			dwStyle &= ~WS_OVERLAPPEDWINDOW;
			dwStyle |= WS_POPUPWINDOW;

			return SetStyle(dwStyle);
		}
		else
		{
			RestoreStyle();
		}
	}

	bool WinApiWindow::IsNoResizeAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsNoResize() const
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);
		return !(dwStyle & WS_THICKFRAME);
	}

	bool WinApiWindow::SetNoResize(bool noResize)
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);

		if(noResize)
			dwStyle &= ~WS_THICKFRAME;
		else
			dwStyle |= WS_THICKFRAME;

		return SetStyle(dwStyle);
	}

	bool WinApiWindow::IsInvisibleAvailable() const
	{
		return true;
	}

	bool WinApiWindow::IsInvisible() const
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);
		return ~(dwStyle & WS_VISIBLE);
	}

	bool WinApiWindow::SetInvisible(bool invisible)
	{
		DWORD dwStyle = GetWindowLongA(mHwnd, GWL_STYLE);

		if (invisible)
			dwStyle &= ~WS_VISIBLE;
		else
			dwStyle |= WS_VISIBLE;

		return SetStyle(dwStyle);
	}

	bool WinApiWindow::SetStyle(DWORD dwStyle)
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

	void WinApiWindow::SetRestoreStyle(DWORD dwStyle)
	{
		mRestoreStyle = dwStyle;
	}

	bool WinApiWindow::RestoreStyle()
	{
		return SetStyle(mRestoreStyle);
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

