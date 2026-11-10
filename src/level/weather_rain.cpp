#include "level/weather_rain.h"

void WeatherRain::update() {
	y += dy;

	if (x < level().camera.offset || (y > -100 && level().terrain.solid_wall(x, 0)) || level().terrain.solid_wall(x, y) || y > 540) destroyed = true;
}
