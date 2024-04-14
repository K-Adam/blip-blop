#pragma once

#include "entities/sprite.h"

class FondPokeCaissePlateforme : public Sprite {
   private:
	int y_cible;

   public:
	FondPokeCaissePlateforme(int niveau_y);
	virtual void update();
};
