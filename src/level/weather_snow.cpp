#include "level/weather_snow.h"
#include <cmath>
#include <numbers>

void WeatherSnow::update() {
	phi += 3;
	phi %= 360;

	xbase -= 2;

	x = xbase + int(xwide * std::cos(phi * std::numbers::pi / 180));
	y += dy;

	if (y > -25 && y < -10 && solid_wall(x, 0)) {
		destroyed = true;
	} else if ((platform_y(x, y) != 0 && ((rand() % 6) == 0)) || solid_wall(x, y)) {
		destroyed = true;
	} else if (y > 500) {
		destroyed = true;
	} else if (xbase < offset - 10) {
		if (!solid_wall(offset + 640, 0))
			xbase += 660;
		else
			destroyed = true;
	}
}
