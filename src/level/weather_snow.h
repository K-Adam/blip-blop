#pragma once

#include "entities/sprite.h"

class WeatherSnow : public Sprite {
   public:
	int xbase;
	int phi;
	int xwide;

	void update();
};
