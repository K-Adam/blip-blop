#include "entities/bubble.h"

#include <cmath>
#include <numbers>

Bubble::Bubble() : phi(0) {}

void Bubble::update() {
	phi += dphi;
	phi %= 360;

	y += dy;

	x = xbase + static_cast<int>(10 * std::cos(phi * std::numbers::pi / 180));

	if (xbase < level().camera.offset - 10 || y < -10 || level().terrain.solid_wall(x, y)) destroyed = true;
}
