#include "dd_gfx.h"
#include "ben_debug.h"
#include "config.h"
#include "globals.h"

Graphics* graphicInstance = NULL;

static int oldRefreshRate = -1;

void DDInitDirectDraw() {
	if (graphicInstance != NULL) {
		debug << "Graphic engine already initialized!\n";
	}
	graphicInstance = new Graphics();
	graphicInstance->Init();
}

void DDCloseDirectDraw() {
	delete graphicInstance;
	graphicInstance = NULL;
}

void DDSetGfxMode(int x, int y, int d, bool fullscreen) {
	debug << "DDSetGfxMode (" << x << "," << y << ")"
		  << "\n";
	graphicInstance->SetGfxMode(x, y, d, fullscreen);
}

SDL::Surface* DDCreateSurface(int x, int y) {
	debug << "DDCreateSurface (" << x << "," << y << ")"
		  << "\n";
	return graphicInstance->CreateSurface(x, y, DDSURF_BEST);
}

SDL::Surface* DDCreateSurface(int x, int y, int flags) { return graphicInstance->CreateSurface(x, y, flags); }

SDL::Surface* DDCreatePrimary() {
	debug << "DDCreatePrimary ()"
		  << "\n";
	return graphicInstance->CreatePrimary();
}

SDL::Surface* DDCreatePrimary(SDL::Surface*& back) {
	debug << "DDCreatePrimary (back)"
		  << "\n";
	return graphicInstance->CreatePrimary(back);
}

SDL::Surface* DDLoadBMP(char* file) {
	debug << "DDLoadBMP (" << file << ")"
		  << "\n";
	return graphicInstance->LoadBMP(file, DDSURF_BEST);
}

SDL::Surface* DDLoadBMP(char* file, int flags) {
	debug << "DDLoadBMP (" << file << ")"
		  << "\n";
	return graphicInstance->LoadBMP(file, flags);
}

bool DDSetColorKey(SDL::Surface* surf, Pixel rgb) { return graphicInstance->SetColorKey(surf, rgb); }

void DDFlip() { graphicInstance->Flip(); }

void DDFlipV() { graphicInstance->FlipV(); }

void DDToggleFullscreen() { graphicInstance->ToggleFullscreen(); }
void DDSetVSync(bool on) { graphicInstance->SetVSync(on); }
