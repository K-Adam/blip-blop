#pragma once

#include "entities/sprite.h"

class Explosion : public Sprite {
   public:
	int type;
	bool fini;
	int speed;

	Explosion();
	void update() override;
};
