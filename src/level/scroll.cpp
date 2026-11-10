#include "level/scroll.h"

#include "core/assets.h"
#include "engine/graphics.h"
#include "level/world.h"

void ScrollRenderer::init() { buffer_.reset(graphics.CreateSurface(WIDTH, 480)); }

void ScrollRenderer::reset() {
	next_x_ = 0;
	x_tex_ = 0;
	n_img_ = 0;
	n_cache_ = 0;
}

void ScrollRenderer::draw(Camera& camera, const Terrain& terrain) {
	Rect r;

	// The cache lives in a render target, rebuild it from the first screen if the driver wiped it
	if (graphics.TakeTargetsLost()) {
		reset();
	}

	camera.clamp((int)terrain.width);

	int x1 = camera.offset % WIDTH;
	int x2 = (camera.offset + 640) % WIDTH;
	int x3 = (camera.offset + WIDTH - 2) % WIDTH;

	r.top = 0;
	r.bottom = 480;

	while (n_img_ < ((camera.offset + WIDTH - 2) / 640) || (n_img_ < (int)terrain.screens && (next_x_ != ((x3 + 1) % WIDTH)) && (next_x_ != ((x3) % WIDTH)))) {
		r.left = x_tex_;
		r.right = x_tex_ + 2;

		assets().pics.decor.anim("screens")[terrain.decor[n_img_]]->BlitPart(buffer_.get(), next_x_, 0, r);

		x_tex_ += 2;

		if (x_tex_ == 640) {
			x_tex_ = 0;
			n_img_ += 1;
		}

		next_x_ += 2;

		if (next_x_ == WIDTH) {
			next_x_ = 0;
			n_cache_ += 1;
		}
	}

	if (x1 <= WIDTH - 640) {
		r.left = x1;
		r.right = x1 + 640;

		graphics.back()->BltFast(0, 0, buffer_.get(), &r);
	} else {
		r.left = x1;
		r.right = WIDTH;

		graphics.back()->BltFast(0, 0, buffer_.get(), &r);

		r.left = 0;
		r.right = x2;

		graphics.back()->BltFast(WIDTH - x1, 0, buffer_.get(), &r);
	}
}

static void clippedBlit(SDL::Surface* surf, const Picture* pic, int x, int y, Rect* clip) {
	if (pic == NULL) return;

	Rect r;
	int x1;
	int y1;
	int x2;
	int y2;
	int xs = pic->xSize();
	int ys = pic->ySize();

	x1 = x - pic->xSpot();
	y1 = y - pic->ySpot();

	x2 = x1 + xs;
	y2 = y1 + ys;

	if (x2 < clip->left || x1 > clip->right || y1 > clip->bottom || y2 < clip->top) return;

	if (x1 < clip->left) {
		r.left = clip->left - x1;
		x1 = clip->left;
	} else
		r.left = 0;

	if (y1 < clip->top) {
		r.top = clip->top - y1;
		y1 = clip->top;
	} else
		r.top = 0;

	if (x2 > clip->right)
		r.right = xs - (x2 - clip->right);
	else
		r.right = xs;

	if (y2 > clip->bottom)
		r.bottom = ys - (y2 - clip->bottom);
	else
		r.bottom = ys;

	pic->BlitPart(surf, x1, y1, r);
}

bool ScrollRenderer::stamp(int x, int y, const Picture* pic) {
	Rect r;
	int ni = x / WIDTH;

	r.top = 0;
	r.bottom = 480;

	x %= WIDTH;

	if (ni == n_cache_ - 1) {
		r.left = next_x_;
		r.right = WIDTH;

		clippedBlit(buffer_.get(), pic, x, y, &r);

		r.left = 0;
		r.right = next_x_;

		clippedBlit(buffer_.get(), pic, x - WIDTH, y, &r);

		return true;
	} else if (ni == n_cache_) {
		r.left = 0;
		r.right = next_x_;

		clippedBlit(buffer_.get(), pic, x, y, &r);

		r.left = next_x_;
		r.right = WIDTH;

		clippedBlit(buffer_.get(), pic, x + WIDTH, y, &r);

		return true;
	}

	return false;
}
