#pragma once

#include "Application.h"
#include "WinApi.h"

namespace Quartz
{
	class WinApiApplication : public Application
	{
	public:
		friend class WinApiHelper;

	private:
		HINSTANCE	mInstance;
		WNDCLASSA	mWndClass;
		bool		mUseRawInput;

	public:
		WinApiApplication(const ApplicationInfo& appInfo, HINSTANCE instance, WNDCLASSA wndClass);

		Window* CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo) override;
		void CloseWindow(Window* pWindow) override;
		void DestroyWindow(Window* pWindow) override;

		bool IsRawInputAvailable() const override;
		bool IsRawInputEnabled() const override;
		bool UseRawInput(bool useRawInput) override;

		void Update() override;

		void* GetNativeHandle() override;

		HINSTANCE GetInstance() const;
		WNDCLASSA GetWndClass() const;
	};

	WinApiApplication* CreateWinApiApplication(const ApplicationInfo& appInfo);
	void DestroyWinApiApplication(WinApiApplication* pWinApiApplication);
}