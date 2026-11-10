#include "entities/shots/shot_bb_laser.h"

#include <algorithm>
#include <cmath>
#include <numbers>

static const int BEAM_HALF_WIDTH = 3;

void ShotBBLaser::aim(double angle) {
	angle_ = angle;
	double a = angle * std::numbers::pi / 180;
	ux_ = std::cos(a);
	uy_ = -std::sin(a);
	// Splatters only come in 8 directions
	dir = (int)std::lround((angle + 90) / 45) & 7;

	// Stops at the first wall or past the screen, in 8 px steps
	double px = x, py = y + BEAM_HALF_WIDTH;
	length_ = 0;
	while (px > level().camera.offset - 5 && px < level().camera.offset + 645 && py > -5 && py < 485 && !level().terrain.solid_wall((int)px, (int)py)) {
		px += ux_ * 8;
		py += uy_ * 8;
		length_ += 8;
	}

	x1 = std::min(x, (int)px) - BEAM_HALF_WIDTH;
	x2 = std::max(x, (int)px) + BEAM_HALF_WIDTH;
	y1 = std::min(y, (int)py - BEAM_HALF_WIDTH);
	y2 = std::max(y + 2 * BEAM_HALF_WIDTH, (int)py + BEAM_HALF_WIDTH);
	col_on = true;
	anim_tick = 2;
}

void ShotBBLaser::update() {
	anim_tick -= 1;

	if (anim_tick == 0) destroyed = true;
}

void ShotBBLaser::render() {
	if (length_ <= 0) return;
	// The horizontal beam strip, cropped to the beam and turned around its start
	Rect r{0, 0, length_, 2 * BEAM_HALF_WIDTH};
	assets().pics.bb.anim("laser_beam")[anim_step]->BlitRotatedPart(graphics.back(), x - level().camera.offset, y, r, SDL_Point{0, BEAM_HALF_WIDTH}, -angle_);
	int ex = x + (int)std::lround(ux_ * length_);
	int ey = y + BEAM_HALF_WIDTH + (int)std::lround(uy_ * length_);
	draw(ex, ey, assets().pics.misc.anim("laser_impact")[anim_step]);
}

bool ShotBBLaser::collision(const Sprite* s) const {
	if (!s->col_on) return false;
	if (s->x2 < x1 || s->x1 > x2 || s->y2 < y1 || s->y1 > y2) return false;
	for (int d = 0; d <= length_; d += 4) {
		double px = x + ux_ * d, py = y + BEAM_HALF_WIDTH + uy_ * d;
		if (px >= s->x1 - BEAM_HALF_WIDTH && px <= s->x2 + BEAM_HALF_WIDTH && py >= s->y1 - BEAM_HALF_WIDTH && py <= s->y2 + BEAM_HALF_WIDTH) return true;
	}
	return false;
}
