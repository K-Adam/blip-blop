#pragma once

#include "graphics.h"
#include <SDL2/SDL.h>

#include "sdl_pixelformat.h"
#include "sdl_surfaceinfo.h"

#ifndef DDLOCK_WAIT	 // not used by SDL
#define DDLOCK_SURFACEMEMORYPTR 0x1
#define DDLOCK_WAIT 0x2
#define DDLOCK_WRITEONLY 0x4
#endif

namespace SDL {

class Surface {
   private:
	SDL_Surface *surface;
	Surface *backBuffer;

   public:
	Surface(SDL_Surface *surf) {
		SDL_SetSurfaceAlphaMod(surf, 0xff);
		surface = surf;
		backBuffer = 0;
	}

	Surface(SDL_Surface *surf, Surface *bb) {
		SDL_SetSurfaceAlphaMod(surf, 0xff);
		surface = surf;
		backBuffer = bb;
	}

	inline void SetBackBuffer(Surface *b) { backBuffer = b; }

	inline SDL_Surface *Get() { return surface; };
	inline void BltFast(int x, int y, SDL::Surface *surf /*This is the Source Surface! Damn, DD!*/, Rect *r, int flags = 0) {
		(void)flags;

		SDL_Rect rect, position;
		position.x = x;
		position.y = y;
		if (r == 0) {
			SDL_BlitSurface(surf->Get(), 0, surface, &position);
		} else {
			rect.x = r->left;
			rect.y = r->top;
			rect.w = r->right - r->left;
			rect.h = r->bottom - r->top;
			/*SDL_BlitSurface(SDL_Surface*    src,
			const SDL_Rect* srcrect,
			SDL_Surface*    dst,
			SDL_Rect*       dstrect)*/
			int ret = SDL_BlitSurface(surf->Get(), &rect, surface, &position);
			if (ret != 0) {
				debug << "Errore SDL_BlitSurface in sdl_surface.h - " << SDL_GetError() << "\n";
				exit(0);
			}
		}
	}

	inline void Blt(Rect *src, SDL::Surface *surf, Rect *dest, int flags = 0, DDBLTFX *pad = 0) {
		(void)flags;
		/*
		TODO: IF flags contains DDBLT_COLORFILL then i must fill the surface with the color of
		*/
		SDL_Rect r = src ? SDL_Rect{src->left, src->top, src->right - src->left, src->bottom - src->top} : SDL_Rect{};
		SDL_Rect p = dest ? SDL_Rect{dest->left, dest->top, dest->right - dest->left, dest->bottom - dest->top} : SDL_Rect{};
		SDL_Rect *rect = src ? &r : nullptr, *position = dest ? &p : nullptr;
		if (surf) {
			SDL_BlitSurface(surf->Get(), rect, surface, position);
		} else {
			if (pad)
				SDL_FillRect(surface, rect, pad->dwFillColor);
			else
				SDL_FillRect(surface, rect, 0x000000);
		}
	}

	inline void Release() {
		SDL_FreeSurface(surface);
		delete this;  // FIXME: OH MY THAT'S DANGEROUS
	}

	inline void FillRect(Rect *r, unsigned int color) {
		if (!r) {
			SDL_FillRect(surface, 0, color);
		} else {
			SDL_Rect rect;
			rect.x = r->left;
			rect.y = r->top;
			rect.w = r->right - r->left;
			rect.h = r->bottom - r->top;
			SDL_FillRect(surface, &rect, color);
		}
	}

	bool GetPixelFormat(SDL::PixelFormat *format) {
		*format = SDL::PixelFormat(surface->format);
		return true;
	}

	bool Lock(SDL::SurfaceInfo *info, int flags, void *) {
		(void)flags;
		if (SDL_LockSurface(surface) != 0) {
			return false;
		}
		info->lpSurface = surface->pixels;
		info->lPitch = surface->pitch;
		return true;
	}

	void Unlock() { SDL_UnlockSurface(surface); }
};

};	// namespace SDL
