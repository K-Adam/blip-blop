#pragma once

#include "entities/sprite.h"

class BackgroundMonolith : public Sprite {
   public:
	int ss_etape_levitation;
	int etape_levitation;

	BackgroundMonolith();
	void update() override;
};
