#pragma once

#include "entities/character.h"

class Splatter : public Sprite {
   public:
	Character *parent;
	int deltax;
	int deltay;

	void update() override;
};
