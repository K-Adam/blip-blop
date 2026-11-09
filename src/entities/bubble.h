#pragma once

#include "entities/sprite.h"

class Bubble : public Sprite {
   public:
	int xbase;
	int phi;
	int dphi;

	Bubble();
	void update() override;
};
