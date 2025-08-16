#pragma once

#include "entities/sprite.h"

class BackgroundRock2 : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	BackgroundRock2();
	void update() override;
};
