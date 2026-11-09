#pragma once

#include "entities/sprite.h"

class Explosion : public Sprite {
   public:
	int type;
	bool finished;
	int speed;

	Explosion();
	void update() override;
};
