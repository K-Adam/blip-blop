#pragma once

#include "entities/sprite.h"

class BackgroundRock1 : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	BackgroundRock1();
	void update() override;
};
