#pragma once

#include "entities/sprite.h"

class BackgroundStatic : public Sprite {
   public:
	virtual void update() {
		if (x < 320) a_detruire = true;
	};
};
