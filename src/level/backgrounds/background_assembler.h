#pragma once

#include "entities/sprite.h"

class BackgroundAssembler : public Sprite {
   public:
	BackgroundAssembler();
	virtual void update();
	virtual void affiche();
};
