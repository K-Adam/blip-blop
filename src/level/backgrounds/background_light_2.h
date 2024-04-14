#pragma once

#include "entities/sprite.h"

class BackgroundLight2 : public Sprite {
   public:
	bool allume;
	int wait_for_cligno;
	int delai_cligno;

	BackgroundLight2();

	virtual void update();
};
