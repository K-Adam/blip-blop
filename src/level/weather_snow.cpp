#include "level/weather_snow.h"
#include <cmath>
#include <numbers>

void WeatherSnow::update() {
	phi += 3;
	phi %= 360;

	xbase -= 2;

	x = xbase + int(xwide * std::cos(phi * std::numbers::pi / 180));
	y += dy;

	if (y > -25 && y < -10 && mur_opaque(x, 0)) {
		a_detruire = true;
	} else if ((plat(x, y) != 0 && ((rand() % 6) == 0)) || mur_opaque(x, y)) {
		a_detruire = true;
	} else if (y > 500) {
		a_detruire = true;
	} else if (xbase < offset - 10) {
		if (!mur_opaque(offset + 640, 0))
			xbase += 660;
		else
			a_detruire = true;
	}
}
