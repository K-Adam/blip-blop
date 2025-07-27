#pragma once

#define WANTED_VBUFFER_WIDE 840

extern int vbuffer_wide;
extern int next_x;
extern int xTex;
extern int n_img;
extern int n_cache;

void updateScrolling(bool forceOk = true);
void drawScrolling();
