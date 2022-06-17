#pragma once

#include "Surface.h"

namespace Quartz
{
	class GLSurface : public Surface
	{
	public:
		bool IsHDR() const override;
	};
}