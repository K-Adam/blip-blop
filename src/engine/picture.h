#pragma once

#include "engine/dd_gfx.h"

#define XPIC_MAX 640  // Représente la largeur de la Primary Surface
#define YPIC_MAX 480  // Représente la hauteur de la Primary Surface

class Picture {
   private:
	SDL::Surface *surf;	 // Surface contenant l'image
	int xsize;			 // Largeur de l'image
	int ysize;			 // Hauteur de l'image
	int xspot;			 // X du point chaud
	int yspot;			 // Y du point chaud

	void FindSize();

   public:
	Picture();

	inline int xSize() const { return xsize; };
	inline int ySize() const { return ysize; };
	inline int xSpot() const { return xspot; };
	inline int ySpot() const { return yspot; };
	inline SDL::Surface *Surf() const { return surf; };

	void SetSpot(int x, int y);
	void SetColorKey(Pixel rgb);
	void SetSurface(SDL::Surface *s);

	void LoadBMP(char *file);
	void LoadBMP(char *file, int flags);
	void LoadBMP(char *file, int xs, int ys);
	void LoadBMP(char *file, int xs, int ys, int flags);
	void LoadLGX(char *file, int xs = 0, int ys = 0, int flags = 0);
	void PasteTo(SDL::Surface *s, int x, int y) const;
	void BlitTo(SDL::Surface *s, int x, int y) const;
	// Rotated counterclockwise around the hotspot, in degrees
	void BlitRotated(SDL::Surface *s, int x, int y, double deg) const;
	void Close();

	~Picture();
};
