#pragma once

#include "entities/sprite.h"

class BackgroundPokeCratePlatform : public Sprite {
   private:
	int y_cible;

   public:
	BackgroundPokeCratePlatform(int niveau_y);
	void update() override;
};
