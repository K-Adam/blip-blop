#include "level/weather_snow.h"
#include <cmath>
#include <numbers>

void WeatherSnow::update() {
	phi += 3;
	phi %= 360;

	xbase -= 2;

	x = xbase + static_cast<int>(xwide * std::cos(phi * std::numbers::pi / 180));
	y += dy;

	if (y > -25 && y < -10 && level().terrain.solid_wall(x, 0)) {
		destroyed = true;
	} else if ((level().terrain.platform_y(x, y) != 0 && ((rnd() % 6) == 0)) || level().terrain.solid_wall(x, y)) {
		destroyed = true;
	} else if (y > 500) {
		destroyed = true;
	} else if (xbase < level().camera.offset - 10) {
		if (!level().terrain.solid_wall(level().camera.offset + 640, 0))
			xbase += 660;
		else
			destroyed = true;
	}
}
