#pragma once

#include "sprite.h"

class FondLumiere2 : public Sprite {
   public:
	bool allume;
	int wait_for_cligno;
	int delai_cligno;

	FondLumiere2();

	virtual void update();
};
