#pragma once

#include "Application.h"

namespace Quartz
{
	class GLFWApplication : public Application
	{
	public:
		friend class GLFWHelper;

	public:
		GLFWApplication(const ApplicationInfo& appInfo);

		bool Create() override;
		void Destroy() override;

		Window* CreateWindow(const WindowInfo& info, const SurfaceInfo& surfaceInfo) override;
		void CloseWindow(Window* pWindow) override;
		void DestroyWindow(Window* pWindow) override;

		bool IsRawInputAvailable() const override;
		bool IsRawInputEnabled() const override;
		bool UseRawInput(bool useRawInput) override;

		void Update() override;
		
		void* GetNativeHandle() override;
	};
}