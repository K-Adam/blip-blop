#include "engine/sdl_surface.h"

#include <algorithm>
#include <memory>

#include "core/debug.h"
#include "engine/graphics.h"

namespace SDL {

static SDL_Renderer* renderer() { return graphics.renderer(); }

Surface::Surface(SDL_Surface* image) : texture_(SDL_CreateTextureFromSurface(renderer(), image)), w_(image->w), h_(image->h) {
	SDL_FreeSurface(image);
	if (!texture_) debug << "Cannot create texture: " << SDL_GetError() << "\n";
	SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
}

Surface::Surface(int width, int height)
	: texture_(SDL_CreateTexture(renderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height)), w_(width), h_(height) {
	if (!texture_) debug << "Cannot create render target: " << SDL_GetError() << "\n";
	// Copies between targets blend on alpha like the software surfaces did
	SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
	FillRect(nullptr, 0xFF000000);
}

Surface::~Surface() {
	if (SDL_GetRenderTarget(renderer()) == texture_) SDL_SetRenderTarget(renderer(), nullptr);
	SDL_DestroyTexture(texture_);
}

void Surface::Target() {
	if (SDL_GetRenderTarget(renderer()) != texture_) SDL_SetRenderTarget(renderer(), texture_);
}

void Surface::BltFast(int x, int y, Surface* src, Rect* r) {
	SDL_Rect s = r ? SDL_Rect{r->left, r->top, r->right - r->left, r->bottom - r->top} : SDL_Rect{0, 0, src->w_, src->h_};
	SDL_Rect d{x, y, 0, 0};
	// Clip the source to the image and move the destination with it, RenderCopy would stretch instead
	if (s.x < 0) {
		d.x -= s.x;
		s.w += s.x;
		s.x = 0;
	}
	if (s.y < 0) {
		d.y -= s.y;
		s.h += s.y;
		s.y = 0;
	}
	if (s.x + s.w > src->w_) s.w = src->w_ - s.x;
	if (s.y + s.h > src->h_) s.h = src->h_ - s.y;
	if (s.w <= 0 || s.h <= 0) return;
	d.w = s.w;
	d.h = s.h;

	if (src == this) {
		// A target cannot be read while it is drawn to, so overlapping copies go through a scratch copy
		static std::unique_ptr<Surface> scratch;
		if (!scratch || scratch->w_ < w_ || scratch->h_ < h_)
			scratch = std::make_unique<Surface>(std::max(w_, scratch ? scratch->w_ : 0), std::max(h_, scratch ? scratch->h_ : 0));
		scratch->Target();
		SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_NONE);
		SDL_RenderCopy(renderer(), texture_, &s, &s);
		SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
		src = scratch.get();
	}
	Target();
	SDL_RenderCopy(renderer(), src->texture_, &s, &d);
}

void Surface::BltRotated(int x, int y, Surface* src, SDL_Point center, double clockwise_deg) {
	Target();
	SDL_Rect d{x, y, src->w_, src->h_};
	SDL_RenderCopyEx(renderer(), src->texture_, nullptr, &d, clockwise_deg, &center, SDL_FLIP_NONE);
}

void Surface::FillRect(Rect* r, unsigned int color) {
	Target();
	SDL_SetRenderDrawBlendMode(renderer(), SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer(), color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, color >> 24);
	if (!r) {
		SDL_RenderFillRect(renderer(), nullptr);
		return;
	}
	SDL_Rect rect{r->left, r->top, r->right - r->left, r->bottom - r->top};
	// Inverted rects fill nothing, as SDL_FillRect did
	if (rect.w > 0 && rect.h > 0) SDL_RenderFillRect(renderer(), &rect);
}

void Surface::Darken(Rect* r) {
	Target();
	SDL_SetRenderDrawBlendMode(renderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer(), 0, 0, 0, 128);
	SDL_Rect rect{r->left, r->top, r->right - r->left, r->bottom - r->top};
	if (rect.w > 0 && rect.h > 0) SDL_RenderFillRect(renderer(), &rect);
}

void Surface::ReadPixels(void* pixels) {
	Target();
	SDL_RenderReadPixels(renderer(), nullptr, SDL_PIXELFORMAT_ABGR8888, pixels, w_ * 4);
}

}  // namespace SDL
