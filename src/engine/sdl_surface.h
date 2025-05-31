#pragma once

#include <SDL2/SDL.h>

#include "core/debug.h"
#include "engine/sdl_surface_info.h"

struct Rect {
	int left;
	int top;
	int right;
	int bottom;
};

#ifndef DDLOCK_WAIT	 // not used by SDL
#define DDLOCK_SURFACEMEMORYPTR 0x1
#define DDLOCK_WAIT 0x2
#define DDLOCK_WRITEONLY 0x4
#endif

namespace SDL {
class Surface {
   private:
	SDL_Surface *surface;

   public:
	Surface(SDL_Surface *surf) {
		SDL_SetSurfaceAlphaMod(surf, 0xff);
		surface = surf;
	}

	inline SDL_Surface *Get() { return surface; };
	inline void BltFast(int x, int y, SDL::Surface *surf, Rect *r) {
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
			int ret = SDL_BlitSurface(surf->Get(), &rect, surface, &position);
			if (ret != 0) {
				debug << "SDL_BlitSurface failed: " << SDL_GetError() << "\n";
			}
		}
	}

	inline void Release() {
		SDL_FreeSurface(surface);
		delete this;
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
