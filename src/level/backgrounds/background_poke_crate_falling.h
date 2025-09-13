#pragma once

#include "entities/sprite.h"

#define UNLOCK_DELAY 32

class BackgroundPokeCrateFalling : public Sprite {
   public:
	BackgroundPokeCrateFalling();
	void update() override;
	void render() override;
};
