#pragma once

#include "entities/sprite.h"

class BackgroundRock2 : public Sprite {
   public:
	int levitation_tick;
	int levitation_step;

	BackgroundRock2();
	void update() override;
};
