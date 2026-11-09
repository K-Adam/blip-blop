#pragma once

#include "entities/sprite.h"

class BackgroundPokeCratePlatform : public Sprite {
   private:
	int y_target;

   public:
	BackgroundPokeCratePlatform(int target_y);
	void update() override;
};
