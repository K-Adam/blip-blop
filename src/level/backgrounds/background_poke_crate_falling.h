#pragma once

#include "entities/sprite.h"

#define UNLOCK_DELAY 32

class FondPokeCaisseTombante : public Sprite {
   public:
	FondPokeCaisseTombante();
	virtual void update();
	virtual void affiche();
};
