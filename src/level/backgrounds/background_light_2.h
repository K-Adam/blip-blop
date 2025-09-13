#pragma once

#include "entities/sprite.h"

class BackgroundLight2 : public Sprite {
   public:
	bool allume;
	int wait_for_blink;
	int delai_cligno;

	BackgroundLight2();

	void update() override;
};
