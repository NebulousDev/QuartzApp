#pragma once

#include "DLL.h"
#include "Types/Types.h"
#include "Math/Vector.h"

namespace Quartz
{
	enum SurfaceAPI
	{
		SURFACE_API_OPENGL,
		SURFACE_API_VULKAN,
		SURFACE_API_DX12,
	};

	struct SurfaceInfo
	{
		SurfaceAPI	surfaceApi;
		bool		requestFullscreen;
		bool		requestHDR;
		void*		pApiInfo;
	};

	class QUARTZAPP_API Surface
	{
	public:
		virtual bool IsHDR() const = 0;
	};
}