#pragma once

#include <memory>

#include "engine/sdl_surface.h"

class Picture;
class Terrain;
struct Camera;

// Ring buffer of decor columns, filled two pixels at a time as the camera moves right
class ScrollRenderer {
   public:
	static constexpr int WIDTH = 840;

	void init();
	void reset();
	void draw(Camera& camera, const Terrain& terrain);
	// Stamps pic into the cached decor, false when x is not cached
	bool stamp(int x, int y, const Picture* pic);

   private:
	std::unique_ptr<SDL::Surface> buffer_;
	int next_x_ = 0;
	int x_tex_ = 0;
	int n_img_ = 0;
	int n_cache_ = 0;
};
