#pragma once

#include "Surface.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	struct VulkanSurfaceInfo
	{
		VkInstance			instance;
		VkPhysicalDevice	physicalDevice;
		VkSurfaceFormatKHR	surfaceFormat;
		bool				exclusiveFullscreen;
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

		bool IsFullscreen() const override;
		bool IsHDR() const override;
	};
}