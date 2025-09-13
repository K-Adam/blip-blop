#pragma once

#include "entities/sprite.h"

class BackgroundMonolith : public Sprite {
   public:
	int levitation_tick;
	int levitation_step;

	BackgroundMonolith();
	void update() override;
};
