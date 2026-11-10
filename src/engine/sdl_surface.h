#pragma once

#include <SDL2/SDL.h>

struct Rect {
	int left;
	int top;
	int right;
	int bottom;
};

namespace SDL {

// A texture. Loaded images are static, surfaces made by Graphics are render targets that keep their pixels between frames
class Surface {
   public:
	// Uploads the image and frees it
	explicit Surface(SDL_Surface *image);
	// Opaque black render target
	Surface(int width, int height);
	~Surface();
	Surface(const Surface &) = delete;
	Surface &operator=(const Surface &) = delete;

	int w() const { return w_; }
	int h() const { return h_; }
	SDL_Texture *Get() const { return texture_; }

	void BltFast(int x, int y, Surface *src, Rect *r);
	void BltRotated(int x, int y, Surface *src, Rect *r, SDL_Point center, double clockwise_deg);
	void BltStretched(const SDL_Rect &dst, Surface *src, const SDL_Rect &r);
	// color is 0xAABBGGRR, written as is without blending
	void FillRect(Rect *r, unsigned int color);
	void Darken(Rect *r);
	void ReadPixels(void *pixels);

	void Release() { delete this; }

   private:
	void Target();

	SDL_Texture *texture_;
	int w_;
	int h_;
};

};	// namespace SDL
