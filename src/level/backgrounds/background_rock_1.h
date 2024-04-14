#pragma once

#include "entities/sprite.h"

class FondRocher1 : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	FondRocher1();
	virtual void update();
};
