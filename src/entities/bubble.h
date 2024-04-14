#pragma once

#include "entities/sprite.h"

class Bubble : public Sprite {
   public:
	int xbase;
	int phi;
	int dphi;

	Bubble();
	virtual void update();
};
