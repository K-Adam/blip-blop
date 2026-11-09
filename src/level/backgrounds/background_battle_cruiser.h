#pragma once

#include "entities/sprite.h"

#define SHOT_SPEED 2

class BackgroundBattleCruiser : public Sprite {
   public:
	int x_opponent;
	int y_opponent;
	int attack_step;
	int fire_step;
	int fire_tick;

	BackgroundBattleCruiser();

	void update() override;
	void render() override;
};
