#pragma once

#include <SDL2/SDL.h>

namespace SDL {

// Stands in for DirectDraw's DDSURFACEDESC
struct SurfaceInfo {
	unsigned int lPitch;
	void* lpSurface;
};

};	// namespace SDL
