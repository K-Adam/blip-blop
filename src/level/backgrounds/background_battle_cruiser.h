#pragma once

#include "entities/sprite.h"

#define TIR_SPEED 2

class BackgroundBattleCruiser : public Sprite {
   public:
	int x_adversaire;
	int y_adversaire;
	int attack_etape;
	int feu_etape;
	int feu_ss_etape;

	BackgroundBattleCruiser();

	virtual void update();
	virtual void affiche();
};
