#pragma once

#include "entities/sprite.h"

class BackgroundAssembler : public Sprite {
   public:
	BackgroundAssembler();
	void update() override;
	void affiche() override;
};
