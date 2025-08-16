#pragma once

#include "entities/sprite.h"

class BackgroundPokeSky : public Sprite {
   public:
	BackgroundPokeSky();
	void update() override;
	void affiche() override;
};
