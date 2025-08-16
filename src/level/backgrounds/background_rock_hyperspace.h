#pragma once

#include "entities/sprite.h"

class BackgroundRockHyperspace : public Sprite {
   public:
	int plan_rocher;
	int placer;
	int delta;
	int x_delay;
	int sinusoide_delay;
	int y_base;

	BackgroundRockHyperspace();
	void update() override;
	void affiche() override;
};
