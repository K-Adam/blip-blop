#include "entities/bubble.h"

#include <cmath>
#include <numbers>

Bubble::Bubble() : phi(0) {}

void Bubble::update() {
	phi += dphi;
	phi %= 360;

	y += dy;

	x = xbase + int(10 * std::cos(phi * std::numbers::pi / 180));

	if (xbase < offset - 10 || y < -10 || solid_wall(x, y)) destroyed = true;
}
