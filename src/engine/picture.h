#pragma once

#include "engine/graphics.h"

#define XPIC_MAX 640
#define YPIC_MAX 480

class Picture {
   private:
	SDL::Surface *surf;
	int xsize;
	int ysize;
	int xspot;
	int yspot;

	void FindSize();

   public:
	Picture();

	inline int xSize() const { return xsize; };
	inline int ySize() const { return ysize; };
	inline int xSpot() const { return xspot; };
	inline int ySpot() const { return yspot; };
	inline SDL::Surface *Surf() const { return surf; };

	void SetSpot(int x, int y);
	void SetSurface(SDL::Surface *s);

	void PasteTo(SDL::Surface *s, int x, int y) const;
	void BlitTo(SDL::Surface *s, int x, int y) const;
	// Rotated counterclockwise around the hotspot, in degrees
	void BlitRotated(SDL::Surface *s, int x, int y, double deg) const;
	void Close();

	~Picture();
};
