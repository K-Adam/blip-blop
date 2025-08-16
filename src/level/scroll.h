#pragma once

#include "engine/sdl_surface.h"

#define WANTED_VBUFFER_WIDE 840

extern int vbuffer_wide;
// Ring buffer of decor columns the scroll draws from
extern SDL::Surface* videoA;
extern int next_x;
extern int xTex;
extern int n_img;
extern int n_cache;

void updateScrolling(bool forceOk = true);
void drawScrolling();
