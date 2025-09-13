#pragma once

#include "entities/sprite.h"

class BackgroundRockHyperspace : public Sprite {
   public:
	int plan_rocher;
	int placed;
	int delta;
	int x_delay;
	int sinusoide_delay;
	int y_base;

	BackgroundRockHyperspace();
	void update() override;
	void render() override;
};
