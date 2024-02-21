#pragma once

#include "Surface.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	struct VulkanApiSurfaceInfo
	{
		VkInstance			instance;
		VkPhysicalDevice	physicalDevice;
		VkSurfaceFormatKHR	surfaceFormat;
		bool				exclusiveFullscreen;
	};

	class VulkanApiSurface : public Surface
	{
	private:
		VkInstance		vkInstance;
		VkSurfaceKHR	vkSurface;

	public:
		VulkanApiSurface(VkInstance vkInstance, VkSurfaceKHR vkSurface);

		VkInstance GetVkInsance() const;
		VkSurfaceKHR GetVkSurface() const;

		bool IsFullscreen() const override;
		bool IsHDR() const override;
	};
}