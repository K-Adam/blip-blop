#pragma once

#include "entities/sprite.h"

class BackgroundRock1 : public Sprite {
   public:
	int levitation_tick;
	int levitation_step;

	BackgroundRock1();
	void update() override;
};
