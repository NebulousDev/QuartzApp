#include "Vulkan/VulkanSurface.h"

namespace Quartz
{
	VulkanSurface::VulkanSurface(VkInstance vkInstance, VkSurfaceKHR vkSurface)
		: vkInstance(vkInstance), vkSurface(vkSurface) { }

	VkInstance VulkanSurface::GetVkInsance()
	{
		return vkInstance;
	}

	VkSurfaceKHR VulkanSurface::GetVkSurface()
	{
		return vkSurface;
	}

	bool VulkanSurface::IsHDR() const
	{
		return false;
	}
}
