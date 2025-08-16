#include "engine/graphics.h"
#include "engine/picture.h"
#include "core/debug.h"

Picture::Picture() : surf(NULL), xspot(0), yspot(0), xsize(0), ysize(0) {}

void Picture::FindSize() {
	xsize = surf->w();
	ysize = surf->h();
}

void Picture::SetSpot(int x, int y) {
	xspot = x;
	yspot = y;
}

void Picture::SetSurface(SDL::Surface* s) {
	surf = s;
	FindSize();
}

void Picture::BlitTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r);
}

void Picture::PasteTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r);
}

void Picture::Close() {
	if (surf != NULL) surf->Release();
	surf = NULL;
}

Picture::~Picture() { Close(); }

void Picture::BlitRotated(SDL::Surface* s, int x, int y, double deg) const { s->BltRotated(x - xspot, y - yspot, surf, SDL_Point{xspot, yspot}, -deg); }
