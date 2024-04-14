#pragma once

#include "entities/sprite.h"

class BackgroundLight : public Sprite {
   public:
	bool allume;
	int wait_for_cligno;
	int delai_cligno;

	BackgroundLight();

	virtual void update();
};
