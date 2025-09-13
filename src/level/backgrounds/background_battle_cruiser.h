#pragma once

#include "entities/sprite.h"

#define TIR_SPEED 2

class BackgroundBattleCruiser : public Sprite {
   public:
	int x_adversaire;
	int y_adversaire;
	int attack_step;
	int feu_etape;
	int feu_ss_etape;

	BackgroundBattleCruiser();

	void update() override;
	void render() override;
};
