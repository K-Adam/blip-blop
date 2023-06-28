/******************************************************************
 *
 *
 *		----------------
 *		  DD_gfx.h
 *		----------------
 *
 *
 *		Fonctions graphiques PROPRES
 *
 *
 *		Prosper / LOADED -   V 0.1
 *
 *
 *
 ******************************************************************/

#pragma once

//-----------------------------------------------------------------------------
//		Constantes pour la création des surfaces
//-----------------------------------------------------------------------------

#define DDSURF_VIDEO 1   // Mettre la surface en mémoire VIDEO
#define DDSURF_SYSTEM 2  // Mettre la surface en mémoire SYSTEM
#define DDSURF_BEST 4    // Mettre la surf en VIDEO si possible, SYSTEM sinon

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "graphics.h"

//-----------------------------------------------------------------------------
//		Objet 'ddraw' global (avec protection pour gfxutil.cpp)
//-----------------------------------------------------------------------------

extern Graphics* graphicInstance;

//-----------------------------------------------------------------------------
//		Prototypes
//-----------------------------------------------------------------------------

void DDInitDirectDraw();
void DDSetGfxMode(int x, int y, int d, bool fullscreen = true);
void DDCloseDirectDraw();
void DDToggleFullscreen();
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
