#pragma once

#include "entities/sprite.h"

class FondMonolite : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	FondMonolite();
	virtual void update();
};
