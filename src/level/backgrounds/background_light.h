#pragma once

#include "entities/sprite.h"

class BackgroundLight : public Sprite {
   public:
	bool allume;
	int wait_for_blink;
	int delai_cligno;

	BackgroundLight();

	void update() override;
};
