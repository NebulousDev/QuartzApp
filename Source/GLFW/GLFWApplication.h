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

		Window* CreateWindow(const WindowInfo& info) override;
		void DestroyWindow(Window* pWindow) override;

		void Update() override;
		
		void* GetNativeHandle() override;
	};
}