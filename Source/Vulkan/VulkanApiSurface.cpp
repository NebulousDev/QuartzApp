#include "Vulkan/VulkanApiSurface.h"

namespace Quartz
{
	VulkanApiSurface::VulkanApiSurface(VkInstance vkInstance, VkSurfaceKHR vkSurface)
		: vkInstance(vkInstance), vkSurface(vkSurface) { }

	VkInstance VulkanApiSurface::GetVkInsance() const
	{
		return vkInstance;
	}

	VkSurfaceKHR VulkanApiSurface::GetVkSurface() const
	{
		return vkSurface;
	}

	bool VulkanApiSurface::IsFullscreen() const
	{
		return false;
	}

	bool VulkanApiSurface::IsHDR() const
	{
		return false;
	}
}
