#include "OpenGL/GLApiSurface.h"

namespace Quartz
{
	bool GLApiSurface::IsFullscreen() const
	{
		// OpenGL does not natively support exclusive fullscreen
		return false;
	}

	bool GLApiSurface::IsHDR() const
	{
		// OpenGL does not natively support HDR
		return false;
	}
}