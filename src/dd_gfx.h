#pragma once

#define DDSURF_VIDEO 1	 // Mettre la surface en mémoire VIDEO
#define DDSURF_SYSTEM 2	 // Mettre la surface en mémoire SYSTEM
#define DDSURF_BEST 4	 // Mettre la surf en VIDEO si possible, SYSTEM sinon

#include "graphics.h"

extern Graphics* graphicInstance;

void DDInitDirectDraw();
void DDSetGfxMode(int x, int y, int d, bool fullscreen = true);
void DDCloseDirectDraw();
void DDToggleFullscreen();
void DDSetVSync(bool on);
SDL::Surface* DDCreatePrimary();
SDL::Surface* DDCreatePrimary(SDL::Surface*& back);
SDL::Surface* DDCreateSurface(int x, int y);
SDL::Surface* DDCreateSurface(int x, int y, int flags);
SDL::Surface* DDLoadBMP(char* file);
SDL::Surface* DDLoadBMP(char* file, int flags);
void* DDLoadPalette(char* file);
bool DDSetColorKey(SDL::Surface* surf, Pixel rgb);
void DDFlip();
void DDFlipV();
