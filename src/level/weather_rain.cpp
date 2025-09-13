#include "level/weather_rain.h"

void WeatherRain::update() {
	y += dy;

	if (x < offset || (y > -100 && solid_wall(x, 0)) || solid_wall(x, y) || y > 540) destroyed = true;
}
