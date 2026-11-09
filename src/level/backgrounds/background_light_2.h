#pragma once

#include "entities/sprite.h"

class BackgroundLight2 : public Sprite {
   public:
	bool lit;
	int wait_for_blink;
	int blink_interval;

	BackgroundLight2();

	void update() override;
};
