#include "OpenGL/GLSurface.h"

namespace Quartz
{
	bool GLSurface::IsHDR() const
	{
		// OpenGL does not natively support HDR
		return false;
	}
}