#pragma once

#include "engine/graphics.h"

#define XPIC_MAX 640
#define YPIC_MAX 480

// One frame: a rectangle of a texture owned by its bank or font, with the hotspot it is drawn by
class Picture {
   private:
	SDL::Surface *surf;
	SDL_Rect src;
	int xspot;
	int yspot;

   public:
	Picture(SDL::Surface *s, SDL_Rect r, int xs, int ys) : surf(s), src(r), xspot(xs), yspot(ys) {}

	inline int xSize() const { return src.w; };
	inline int ySize() const { return src.h; };
	inline int xSpot() const { return xspot; };
	inline int ySpot() const { return yspot; };

	void BlitTo(SDL::Surface *s, int x, int y) const;
	void PasteTo(SDL::Surface *s, int x, int y) const { BlitTo(s, x, y); }
	// Rotated counterclockwise around the hotspot, in degrees
	void BlitRotated(SDL::Surface *s, int x, int y, double deg) const;
	// Part r of the frame, ignoring the hotspot. Clipped to the frame so it never shows its neighbours
	void BlitPart(SDL::Surface *s, int x, int y, Rect r) const;
	void BlitRotatedPart(SDL::Surface *s, int x, int y, Rect r, SDL_Point center, double clockwise_deg) const;
};
