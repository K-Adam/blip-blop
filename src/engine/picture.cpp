#include <algorithm>
#include <cmath>
#include <numbers>

#include "engine/graphics.h"
#include "engine/picture.h"
#include "engine/dd_gfx.h"
#include "core/debug.h"

Picture::Picture() : surf(NULL), xspot(0), yspot(0), xsize(0), ysize(0) {}

void Picture::FindSize() {
	xsize = surf->Get()->w;
	ysize = surf->Get()->h;
}

void Picture::SetColorKey(Pixel rgb) {
	if (surf == NULL) {
		debug << "Picture::SetColorKey() - surface à NULL\n";
		return;
	}

	DDSetColorKey(surf, rgb);
}

void Picture::SetSpot(int x, int y) {
	xspot = x;
	yspot = y;
}

void Picture::SetSurface(SDL::Surface* s) {
	surf = s;
	FindSize();
}

void Picture::LoadBMP(char* file) {
	surf = DDLoadBMP(file);
	FindSize();
	xspot = 0;
	yspot = 0;
}

void Picture::LoadBMP(char* file, int flags) {
	surf = DDLoadBMP(file, flags);
	FindSize();
	xspot = 0;
	yspot = 0;
}

void Picture::LoadBMP(char* file, int xs, int ys) {
	surf = DDLoadBMP(file);
	FindSize();
	xspot = xs;
	yspot = ys;
}

void Picture::LoadBMP(char* file, int xs, int ys, int flags) {
	surf = DDLoadBMP(file, flags);
	FindSize();
	xspot = xs;
	yspot = ys;
}

void Picture::BlitTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	// S'il n'y a rien à afficher, on se casse tout de suite!
	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

void Picture::PasteTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	// S'il n'y a rien à afficher, on se casse tout de suite!
	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
}

void Picture::Close() {
	if (surf != NULL) surf->Release();
	surf = NULL;
}

Picture::~Picture() { Close(); }

void Picture::BlitRotated(SDL::Surface* s, int x, int y, double deg) const {
	SDL_Surface* src = surf->Get();
	int r = (int)std::ceil(std::hypot(std::max(xspot, xsize - xspot), std::max(yspot, ysize - yspot)));
	SDL_Surface* tmp = SDL_CreateRGBSurfaceWithFormat(0, 2 * r, 2 * r, 32, src->format->format);
	if (!tmp) return;
	Uint32 key;
	if (SDL_GetColorKey(src, &key) == 0) {
		SDL_FillRect(tmp, nullptr, key);
		SDL_SetColorKey(tmp, SDL_TRUE, key);
	}

	double a = deg * std::numbers::pi / 180, c = std::cos(a), sn = std::sin(a);
	SDL_LockSurface(src);
	SDL_LockSurface(tmp);
	for (int dy = -r; dy < r; dy++) {
		Uint32* row = (Uint32*)((Uint8*)tmp->pixels + (dy + r) * tmp->pitch);
		for (int dx = -r; dx < r; dx++) {
			int sx = xspot + (int)std::lround(dx * c - dy * sn);
			int sy = yspot + (int)std::lround(dx * sn + dy * c);
			if (sx >= 0 && sy >= 0 && sx < src->w && sy < src->h) row[dx + r] = ((Uint32*)((Uint8*)src->pixels + sy * src->pitch))[sx];
		}
	}
	SDL_UnlockSurface(tmp);
	SDL_UnlockSurface(src);

	SDL_Rect pos{x - r, y - r, 0, 0};
	SDL_BlitSurface(tmp, nullptr, s->Get(), &pos);
	SDL_FreeSurface(tmp);
}
