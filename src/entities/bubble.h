#pragma once

#include "entities/sprite.h"

class Bulle : public Sprite {
   public:
	int xbase;
	int phi;
	int dphi;

	Bulle();
	virtual void update();
};
