#pragma once

#include "entities/sprite.h"

class BackgroundPokeSky : public Sprite {
   public:
	BackgroundPokeSky();
	virtual void update();
	virtual void affiche();
};
