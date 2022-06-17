#pragma once

#include "Surface.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	struct VulkanSurfaceApiInfo
	{
		VkInstance		vkInstance;
	};

	class VulkanSurface : public Surface
	{
	private:
		VkInstance		vkInstance;
		VkSurfaceKHR	vkSurface;

	public:
		VulkanSurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

		VkInstance GetVkInsance();
		VkSurfaceKHR GetVkSurface();

		bool IsHDR() const override;
	};
}