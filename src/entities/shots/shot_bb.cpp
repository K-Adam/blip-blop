#include "entities/shots/shot_bb.h"

#include <cmath>
#include <numbers>

void ShotBB::setDir(int d) {
	if (d == BBDIR_DOWN_D_L)
		dir = 0;
	else if (d > BBLIM_RIGHT)
		dir = d - 1;
	else
		dir = d;
}

void ShotBB::setAngle(double deg, double speed) {
	dir = (int)std::lround((deg + 90) / 22.5) & 15;
	double a = deg * std::numbers::pi / 180;
	vx_ = std::cos(a) * speed;
	vy_ = -std::sin(a) * speed;
	fx_ = x;
	fy_ = y;
}

void ShotBB::moveAlongAngle() {
	fx_ += vx_ + dxReferentiel;
	fy_ += vy_;
	x = (int)std::lround(fx_);
	y = (int)std::lround(fy_);
}
