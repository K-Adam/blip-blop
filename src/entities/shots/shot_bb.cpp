#include "entities/shots/shot_bb.h"

#include <algorithm>
#include <climits>
#include <cmath>
#include <numbers>

void ShotBB::setAngle(double deg, double speed) {
	angle_ = deg;
	dir = (int)std::lround((deg + 90) / 22.5) & 15;
	double a = deg * std::numbers::pi / 180;
	vx_ = std::cos(a) * speed;
	vy_ = -std::sin(a) * speed;
	fx_ = x;
	fy_ = y;
}

void ShotBB::moveAlongAngle() {
	fx_ += vx_ + dxScroll;
	fy_ += vy_;
	x = (int)std::lround(fx_);
	y = (int)std::lround(fy_);
}

void ShotBB::colAlongAngle() {
	int l = x - pic->xSpot(), t = y - pic->ySpot(), r = l + pic->xSize(), b = t + pic->ySize();
	x1 = y1 = INT_MAX;
	x2 = y2 = INT_MIN;
	for (int cx : {l, r})
		for (int cy : {t, b}) {
			int px = cx, py = cy;
			rotateAround(px, py, x, y, angle_ + 90);
			x1 = std::min(x1, px);
			y1 = std::min(y1, py);
			x2 = std::max(x2, px);
			y2 = std::max(y2, py);
		}
	col_on = true;
}
