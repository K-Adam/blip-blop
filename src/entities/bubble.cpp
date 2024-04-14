#include "entities/bubble.h"

#include "core/math_utils.h"

Bubble::Bubble() : phi(0) {}

void Bubble::update() {
	phi += dphi;
	phi %= 360;

	y += dy;

	x = xbase + cosi(10, phi);

	if (xbase < offset - 10 || y < -10 || mur_opaque(x, y)) a_detruire = true;
}
