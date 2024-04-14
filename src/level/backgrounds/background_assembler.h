#pragma once

#include "entities/sprite.h"

class FondAssembleur : public Sprite {
   public:
	FondAssembleur();
	virtual void update();
	virtual void affiche();
};
