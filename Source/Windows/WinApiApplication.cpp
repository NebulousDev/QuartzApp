#include "WinApiApplication.h"

#include "Types/Types.h"
#include "WinApiHelper.h"
#include "WinApiWindow.h"
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
		DWORD  dwStyle			= WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		DWORD  dwRestoreStyle	= dwStyle;

		uInt32 adjustedWidth	= 0;
		uInt32 adjustedHeight	= 0;
		int32  adjustedPosX		= 0;
		int32  adjustedPosY		= 0;

		if (!(info.hints & WINDOW_FULLSCREEN))
		{
			if (info.hints & WINDOW_BORDERLESS)
			{
				dwStyle	= WS_POPUPWINDOW | WS_VISIBLE;
			}
			if (info.hints & WINDOW_NO_RESIZE)
			{
				dwStyle			&= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
				dwRestoreStyle	&= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
			}
			if (info.hints & WINDOW_INVISIBLE)
			{
				dwRestoreStyle	&= ~WS_VISIBLE;
				dwRestoreStyle	&= ~WS_VISIBLE;
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

			adjustedWidth  = windowSize.right  - windowSize.left;
			adjustedHeight = windowSize.bottom - windowSize.top;
			adjustedPosX   = windowSize.left;
			adjustedPosY   = windowSize.top;
		}
		else
		{
			dwStyle = WS_POPUPWINDOW | WS_VISIBLE;

			adjustedWidth	= info.width;
			adjustedHeight	= info.height;
			adjustedPosX	= 0;
			adjustedPosY	= 0;
		}

		HWND hwnd = CreateWindowExA(0, TEXT(mAppName.Str()), TEXT(info.title.Str()),
			dwStyle, adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight,
			NULL, NULL, mInstance, NULL);

		if (hwnd == NULL)
		{
			printf("Error creating WinApi Window: CreateWindowEx() returned null.\n");
			WinApiHelper::PrintLastError();
			return nullptr;
		}

		// Set fullscreen last so as to not flash if something goes wrong.
		if (info.hints & WINDOW_FULLSCREEN)
		{
			if (!WinApiHelper::SetDisplayMode(0, info.width, info.height, 165))
			{
				printf("Warning while setting WinApi Window to fullscreen mode: SetDisplayMode() failed.\n");
			}
		}

		Surface* pSurface = WinApiHelper::CreateSurface(mInstance, hwnd, surfaceInfo);;
		
		if ((surfaceInfo.surfaceApi != SURFACE_API_NONE) && !pSurface)
		{
			printf("Error creating WinApi Window: Surface generation failed.\n");
			::DestroyWindow(hwnd);
			return nullptr;
		}

		WinApiWindow* pWindow = new WinApiWindow(this, nullptr, dwRestoreStyle, hwnd);

		WinApiHelper::SetWindowOpenState(pWindow, true);

		return pWindow;
	}

	void WinApiApplication::CloseWindow(Window* pWindow)
	{
	}

	void WinApiApplication::DestroyWindow(Window* pWindow)
	{
		delete pWindow->GetSurface(); // TODO
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
		//WinApiApplication* pApp = (WinApiApplication*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//Engine* pEngine = Engine::GetInstance();
		//
		//if (pApp != nullptr)
		//{
		//	Win32Window* pWindow = pApp->mHWNDMap[reinterpret_cast<Handle64>(hwnd)];
		//
		//	if (pWindow != nullptr)
		//	{
		//		switch (uMsg)
		//		{
		//		case WM_DEVICECHANGE:
		//		{
		//			if (wParam == DBT_DEVNODES_CHANGED)
		//			{
		//				pEngine->GetPlatform()->GetPeripheralController()->RescanPeripherals();
		//			}
		//
		//			break;
		//		}
		//
		//		case WM_KILLFOCUS:
		//		{
		//			/*
		//			if (pApp->mWindowFocusCallback)
		//			{
		//				pWindow->focused = false;
		//				pApp->mWindowFocusCallback(pWindow, false);
		//			}
		//
		//			if (pApp->mCapturingWindow)
		//			{
		//				pApp->ReleaseCursor();
		//			}
		//			*/
		//
		//			break;
		//		}
		//
		//		case WM_SETFOCUS:
		//		{
		//			/*
		//			if (pApp->mWindowFocusCallback)
		//			{
		//				pWindow->focused = true;
		//				pApp->mWindowFocusCallback(pWindow, true);
		//			}
		//			*/
		//
		//			break;
		//		}
		//
		//		case WM_MOUSEMOVE:
		//		{
		//			/*
		//			if (!pWindow->mouseInside)
		//			{
		//				if (pApp->mWindowMouseEnteredCallback)
		//				{
		//					pWindow->mouseInside = true;
		//					pApp->mWindowMouseEnteredCallback(pWindow, true);
		//				}
		//			}
		//			*/
		//
		//			break;
		//		}
		//
		//		case WM_MOUSELEAVE:
		//		{
		//			/*
		//			if (pApp->mWindowMouseEnteredCallback)
		//			{
		//				pWindow->mouseInside = false;
		//				pApp->mWindowMouseEnteredCallback(pWindow, false);
		//			}
		//			*/
		//
		//			break;
		//		}
		//
		//		case WM_SIZE:
		//		{
		//			/*
		//			uInt32 x = LOWORD(lParam);
		//			uInt32 y = HIWORD(lParam);
		//
		//			RECT rect, clientRect;
		//			GetWindowRect(hwnd, &rect);
		//			GetClientRect(hwnd, &clientRect);
		//			ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.left));
		//			ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.right));
		//
		//			Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
		//			Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
		//
		//			pWindow->bounds = bounds;
		//			pWindow->clientBounds = clientBounds;
		//
		//			if (pApp->mWindowResizedCallback)
		//			{
		//				pApp->mWindowResizedCallback(pWindow, clientBounds.Width(), clientBounds.Height());
		//			}
		//
		//			if (pApp->mCapturingWindow)
		//			{
		//				// @Note: Recapture mouse to reset clipping
		//				pApp->CaptureCursor(pApp->mCapturingWindow);
		//			}
		//			*/
		//
		//			break;
		//		}
		//		case WM_MOVE:
		//		{
		//			/*
		//			uInt32 x = LOWORD(lParam);
		//			uInt32 y = HIWORD(lParam);
		//
		//			RECT rect, clientRect;
		//			GetWindowRect(hwnd, &rect);
		//			GetClientRect(hwnd, &clientRect);
		//			ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.left));
		//			ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.right));
		//
		//			Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
		//			Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
		//
		//			pWindow->bounds = bounds;
		//			pWindow->clientBounds = clientBounds;
		//
		//			if (pApp->mWindowMovedCallback)
		//			{
		//				pApp->mWindowMovedCallback(pWindow, clientBounds.start.x, clientBounds.start.y);
		//			}
		//
		//			if (pApp->mCapturingWindow)
		//			{
		//				// @Note: Recapture mouse to reset clipping
		//				pApp->CaptureCursor(pApp->mCapturingWindow);
		//			}
		//			*/
		//
		//			break;
		//		}
		//		case WM_CLOSE:
		//		{
		//			WindowCloseEvent closeEvent;
		//			closeEvent.pWindow = pWindow;
		//			pEngine->GetEventSystem()->Publish(closeEvent, EVENT_PRIORITY_IMMEDIATE);
		//			break;
		//		}
		//
		//		case WM_DESTROY:
		//		{
		//			//pApp->ReleaseCursor();
		//
		//			break;
		//		}
		//		default:
		//			break;
		//		}
		//	}
		//}

		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}

	WinApiApplication* CreateWinApiApplication(const ApplicationInfo& appInfo)
	{
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

		return new WinApiApplication(appInfo, hInstance, wndClass);
	}

	void DestroyWinApiApplication(WinApiApplication* pWinApiApplication)
	{

	}
}