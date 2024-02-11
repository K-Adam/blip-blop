#pragma once

#include "sprite.h"

class FondRocher2 : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	FondRocher2();
	virtual void update();
};
