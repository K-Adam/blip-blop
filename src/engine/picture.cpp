#include "engine/picture.h"

#include <algorithm>
#include <cmath>

#include "engine/graphics.h"

void Picture::BlitTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + src.w;
	int y2 = y + src.h;

	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : src.w;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : src.h;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	BlitPart(s, x, y, r);
}

void Picture::BlitRotated(SDL::Surface* s, int x, int y, double deg) const {
	BlitRotatedPart(s, x - xspot, y - yspot, Rect{0, 0, src.w, src.h}, SDL_Point{xspot, yspot}, -deg);
}

void Picture::BlitScaled(SDL::Surface* s, int x, int y, double scale) const {
	auto at = [&](double v) { return int(std::lround(v * scale)); };
	SDL_Rect d{x - at(xspot), y - at(yspot), at(src.w), at(src.h)};
	s->BltStretched(d, surf, src);
}

void Picture::BlitPart(SDL::Surface* s, int x, int y, Rect r) const {
	if (r.left < 0) {
		x -= r.left;
		r.left = 0;
	}
	if (r.top < 0) {
		y -= r.top;
		r.top = 0;
	}
	r.right = std::min(r.right, src.w);
	r.bottom = std::min(r.bottom, src.h);
	if (r.right <= r.left || r.bottom <= r.top) return;
	Rect t{src.x + r.left, src.y + r.top, src.x + r.right, src.y + r.bottom};
	s->BltFast(x, y, surf, &t);
}

void Picture::BlitRotatedPart(SDL::Surface* s, int x, int y, Rect r, SDL_Point center, double clockwise_deg) const {
	r.right = std::min(r.right, src.w);
	r.bottom = std::min(r.bottom, src.h);
	if (r.left < 0 || r.top < 0 || r.right <= r.left || r.bottom <= r.top) return;
	Rect t{src.x + r.left, src.y + r.top, src.x + r.right, src.y + r.bottom};
	s->BltRotated(x, y, surf, &t, center, clockwise_deg);
}
