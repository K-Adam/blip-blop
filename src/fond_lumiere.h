#pragma once

#include "sprite.h"

class FondLumiere : public Sprite {
   public:
	bool allume;
	int wait_for_cligno;
	int delai_cligno;

	FondLumiere();

	virtual void update();
};
