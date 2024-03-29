#pragma once

#include "Surface.h"

namespace Quartz
{
	class GLApiSurface : public Surface
	{
	public:
		bool IsFullscreen() const override;
		bool IsHDR() const override;
	};
}