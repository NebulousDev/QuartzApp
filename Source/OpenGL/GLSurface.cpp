#include "OpenGL/GLSurface.h"

namespace Quartz
{
	bool GLSurface::IsFullscreen() const
	{
		// OpenGL does not natively support exclusive fullscreen
		return false;
	}

	bool GLSurface::IsHDR() const
	{
		// OpenGL does not natively support HDR
		return false;
	}
}