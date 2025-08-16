#pragma once

#include "entities/sprite.h"

class BackgroundStatic : public Sprite {
   public:
	void update() override {
		if (x < 320) a_detruire = true;
	};
};
