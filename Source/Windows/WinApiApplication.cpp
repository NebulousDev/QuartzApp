#include "WinApiApplication.h"

#include "Types/Types.h"
#include "WinApiHelper.h"
#include "WinApiWindow.h"
#include "WinApiRegistry.h"
#include "WinApi.h"

namespace Quartz
{
	WinApiApplication::WinApiApplication(const ApplicationInfo& appInfo, HINSTANCE instance, WNDCLASSA wndClass) :
		Application(appInfo), 
		mInstance(instance),
		mWndClass(wndClass),
		mUseRawInput(false) { }

	Window* WinApiApplication::CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo)
	{
		DWORD       dwStyle      = 0;
		WindowStyle restoreStyle = {};

		dwStyle = WS_OVERLAPPEDWINDOW;

		if (info.hints & WINDOW_BORDERLESS)
		{
			dwStyle = WS_POPUPWINDOW;
			restoreStyle.borderless = true;
		}
		if (info.hints & WINDOW_NO_RESIZE)
		{
			dwStyle &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
			restoreStyle.noResize = true;
		}
		if (info.hints & WINDOW_INVISIBLE)
		{
			restoreStyle.invisible = true;
		}

		LONG posX	= info.posX;
		LONG posY	= info.posY;
		LONG width	= info.width;
		LONG height	= info.height;

		RECT windowSize =
		{
			posX, posY,
			posX + width,
			posY + height
		};

		AdjustWindowRect(&windowSize, dwStyle, FALSE);

		uInt32 adjustedWidth	= windowSize.right - windowSize.left;
		uInt32 adjustedHeight	= windowSize.bottom - windowSize.top;
		int32  adjustedPosX		= windowSize.left;
		int32  adjustedPosY		= windowSize.top;

		HWND hwnd = CreateWindowExA(0, TEXT(mAppName.Str()), TEXT(info.title.Str()),
			dwStyle, adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight,
			NULL, NULL, mInstance, NULL);

		if (hwnd == NULL)
		{
			printf("Error creating WinApi Window: CreateWindowEx() returned null.\n");
			WinApiHelper::PrintLastError();
			return nullptr;
		}

		Surface* pSurface = WinApiHelper::CreateSurface(mInstance, hwnd, surfaceInfo);;
		
		if ((surfaceInfo.surfaceApi != SURFACE_API_NONE) && !pSurface)
		{
			printf("Error creating WinApi Window: Surface generation failed.\n");
			::DestroyWindow(hwnd);
			return nullptr;
		}

		WinApiWindow* pWindow = new WinApiWindow(this, pSurface, restoreStyle, hwnd);

		SetLastError(0);
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
		if (GetLastError())
		{
			printf("Warning while setting WinApi Window user data: SetWindowLongA() failed.\n");
			WinApiHelper::PrintLastError();
		}

		// Set fullscreen last so as to not flash if something goes wrong.
		if (info.hints & WINDOW_FULLSCREEN)
		{
			if (!pWindow->SetFullscreen(true))
			{
				printf("Warning while setting WinApi Window to fullscreen mode: SetDisplayMode() failed.\n");
			}
		}

		if (!(info.hints & WINDOW_INVISIBLE))
		{
			ShowWindow(hwnd, SW_SHOW);
		}

		return pWindow;
	}

	void WinApiApplication::CloseWindow(Window* pWindow)
	{
		WinApiWindow* pWinApiWindow = static_cast<WinApiWindow*>(pWindow);

		if (pWinApiWindow && pWinApiWindow->IsOpen())
		{
			delete pWinApiWindow->GetSurface(); // TODO

			WinApiHelper::SetWindowOpenState(pWinApiWindow, false);
			WinApiHelper::CallWindowClosedCallback(this, pWinApiWindow);

			WinApiRegistry::UnregisterAppWindow(this, pWinApiWindow);
		
			::DestroyWindow(pWinApiWindow->GetHWND());
		}
	}

	void WinApiApplication::DestroyWindow(Window* pWindow)
	{
		CloseWindow(pWindow);
		delete pWindow;
	}

	bool WinApiApplication::IsRawInputAvailable() const
	{
		return false;
	}

	bool WinApiApplication::IsRawInputEnabled() const
	{
		return false;
	}

	bool WinApiApplication::UseRawInput(bool useRawInput)
	{
		return false;
	}

	void WinApiApplication::Update()
	{
		MSG msg = {};

		/*
		// Process and remove all messages before WM_INPUT
		while (PeekMessageW(&msg, NULL, 0, WM_INPUT - 1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		// Process and remove all messages after WM_INPUT
		while (PeekMessageW(&msg, NULL, WM_INPUT + 1, (UINT)-1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (!mUseRawInput)
		{
			// We only want to process WndProc inputs when NOT using raw input 

			// Process all WM_INPUT messages
			while (PeekMessageW(&msg, NULL, WM_INPUT, WM_INPUT, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		*/

		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

	}

	void* WinApiApplication::GetNativeHandle()
	{
		return &mWndClass; // TODO: probably just null
	}

	HINSTANCE WinApiApplication::GetInstance() const
	{
		return mInstance;
	}

	WNDCLASSA WinApiApplication::GetWndClass() const
	{
		return mWndClass;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WinApiWindow* pWindow = (WinApiWindow*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
	
		if (!pWindow)
		{
			return DefWindowProcA(hwnd, uMsg, wParam, lParam);
		}

		WinApiApplication* pApp = (WinApiApplication*)pWindow->GetParentApplication();

		switch (uMsg)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				uInt16 scancode = (uInt8)(lParam >> 16);
				int repeatCount = WinApiHelper::GetKeyRepeatCount(scancode);

				WinApiHelper::CallKeyCallback(pApp, pWindow, scancode, true, repeatCount > 0);

				// HACK
				{
					bool shift = HIBYTE(GetKeyState(VK_SHIFT));
					WinApiHelper::SetKeyRepeatCount(scancode + (shift << 15), repeatCount + 1);

					if (shift)
					{
						int shiftCode = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						int shiftRep = WinApiHelper::GetKeyRepeatCount(shiftCode);
						WinApiHelper::SetKeyRepeatCount(shiftCode, shiftRep + 1);
					}
				}

				break;
			}

			case WM_KEYUP:
			case WM_SYSKEYUP:
			{ 
				uInt16 scancode = (uInt8)(lParam >> 16);

				WinApiHelper::CallKeyCallback(pApp, pWindow, scancode, false, false);

				// HACK
				{
					bool shift = HIBYTE(GetKeyState(VK_SHIFT));
					WinApiHelper::SetKeyRepeatCount(scancode + (shift << 15), 0);

					if (shift)
					{
						int shiftCode = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						WinApiHelper::SetKeyRepeatCount(shiftCode, 0);
					}
				}

				break;
			}

			case WM_CHAR:
			case WM_SYSCHAR:
			{
				char character = (char)wParam;
				uInt16 scancode = (uInt8)(lParam >> 16);

				bool shift = HIBYTE(VkKeyScanA((TCHAR)wParam)) & 1;
				bool ctrl  = HIBYTE(VkKeyScanA((TCHAR)wParam)) & 2;

				// HACK
				int repeatCount = WinApiHelper::GetKeyRepeatCount(scancode + (shift << 15));

				if (!ctrl)
				{
					// NOTE: repeatCount > 1 to compensate for WM_KEYDOWN first increasing repeatCount
					WinApiHelper::CallKeyTypedCallback(pApp, pWindow, character, repeatCount > 1);
				}

				break;
			}

			case WM_MOUSEMOVE:
			{
				WORD mouseX = LOWORD(lParam);
				WORD mouseY = HIWORD(lParam);

				Point2i pos = { (uSize)mouseX, (uSize)mouseY };

				if (!WinApiHelper::IsMouseInside(pWindow))
				{
					WinApiHelper::SetLastMousePos(pWindow, pos);

					TRACKMOUSEEVENT trackEvent;
					trackEvent.cbSize		= sizeof(TRACKMOUSEEVENT);
					trackEvent.hwndTrack	= hwnd;
					trackEvent.dwFlags		= TME_LEAVE;
					trackEvent.dwHoverTime	= 0;

					if (TrackMouseEvent(&trackEvent))
					{
						WinApiHelper::CallMouseEnteredCallback(pApp, pWindow, true);
						WinApiHelper::SetMouseInside(pWindow, true);
					}
				}

				WinApiHelper::CallMouseMovedCallback(pApp, pWindow, mouseX, mouseY);

				Vec2i relative = pos - WinApiHelper::GetLastMousePos(pWindow);
				WinApiHelper::CallMouseMovedRelativeCallback(pApp, pWindow, relative.x, relative.y);
				WinApiHelper::SetLastMousePos(pWindow, pos);

				break;
			}

			case WM_MOUSELEAVE:
			{
				WinApiHelper::CallMouseEnteredCallback(pApp, pWindow, false);
				WinApiHelper::SetMouseInside(pWindow, false);

				break;
			}

			case WM_CLOSE:
			{
				if (lParam == WINAPI_CLOSE_REQUEST_PARAM)
				{
					pApp->CloseWindow(pWindow);
				}
				else
				{
					if (WinApiHelper::CallWindowCloseRequestedCallback(pApp, pWindow))
					{
						pApp->CloseWindow(pWindow);
					}
				}

				return 0;
			}

			case WM_SIZE:
			{
				if (wParam == SIZE_MINIMIZED)
				{
					WinApiHelper::CallWindowMinimizedCallback(pApp, pWindow, true);
					WinApiHelper::SetWindowLastMinimized(pWindow, true);
				}

				if (wParam == SIZE_MAXIMIZED)
				{
					WinApiHelper::CallWindowMaximizedCallback(pApp, pWindow, true);
					WinApiHelper::SetWindowLastMaximized(pWindow, true);
				}

				if (wParam == SIZE_RESTORED)
				{
					if (WinApiHelper::GetWindowLastMinimized(pWindow))
					{
						WinApiHelper::CallWindowMinimizedCallback(pApp, pWindow, false);
						WinApiHelper::SetWindowLastMinimized(pWindow, false);
					}

					if (WinApiHelper::GetWindowLastMaximized(pWindow))
					{
						WinApiHelper::CallWindowMaximizedCallback(pApp, pWindow, false);
						WinApiHelper::SetWindowLastMaximized(pWindow, false);

					}

					UINT width = LOWORD(lParam);
					UINT height = HIWORD(lParam);

					WinApiHelper::CallWindowSizeCallback(pApp, pWindow, width, height);
				}

				break;
			}

			case WM_MOVE:
			{
				DWORD posX = LOWORD(lParam);
				DWORD posY = HIWORD(lParam);

				WinApiHelper::CallWindowPosCallback(pApp, pWindow, posX, posY);

				break;
			}

			case WM_SETFOCUS:
			{
				WinApiHelper::CallWindowFocusedCallback(pApp, pWindow, true);

				if (pWindow->IsFullscreen())
				{
					uSize refreshRate = WinApiHelper::GetDefaultMonitorRefreshRate();

					WinApiHelper::SetDisplayMode(0, pWindow->GetWidth(), pWindow->GetHeight(), refreshRate);
				}

				break;
			}

			case WM_KILLFOCUS:
			{
				WinApiHelper::CallWindowFocusedCallback(pApp, pWindow, false);

				if (pWindow->IsFullscreen())
				{
					Vec2u size = WinApiHelper::GetDefaultMonitorSize();
					uSize refreshRate = WinApiHelper::GetDefaultMonitorRefreshRate();

					WinApiHelper::SetDisplayMode(0, size.x, size.y, refreshRate);
				}

				break;
			}

			case WM_DISPLAYCHANGE:
			{
				// TODO: set WinApiHelper monitor size etc
				break;
			}
		}

		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}

	WinApiApplication* CreateWinApiApplication(const ApplicationInfo& appInfo)
	{
		if (!WinApiHelper::IsWinApiInitialized())
		{
			WinApiHelper::InitializeWinApi();
		}

		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSA wndClass = {};
		wndClass.lpfnWndProc	= WndProc;
		wndClass.hInstance		= hInstance;
		wndClass.lpszClassName	= appInfo.appName.Str();

		ATOM result = RegisterClassA(&wndClass);

		if (result == 0)
		{
			printf("Unable to create application '%s'! RegisterClass() failed.\n", appInfo.appName.Str());
			WinApiHelper::PrintLastError();
			return nullptr;
		}

		WinApiApplication* pApplication = new WinApiApplication(appInfo, hInstance, wndClass);

		WinApiRegistry::RegisterApp(pApplication);

		return pApplication;
	}

	void DestroyWinApiApplication(WinApiApplication* pWinApiApplication)
	{
		// Intentional copy so as to not remove elements from the itterator live
		Array<WinApiWindow*> appWindows = WinApiRegistry::GetWindows(pWinApiApplication);

		for (WinApiWindow* pWindow : appWindows)
		{
			pWinApiApplication->DestroyWindow(pWindow);
		}

		WinApiRegistry::UnregisterApp(pWinApiApplication);

		delete pWinApiApplication;
	}
}