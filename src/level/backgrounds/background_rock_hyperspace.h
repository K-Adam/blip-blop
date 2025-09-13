#pragma once

#include "entities/sprite.h"

class BackgroundRockHyperspace : public Sprite {
   public:
	int rock_layer;
	int placed;
	int delta;
	int x_delay;
	int sinusoid_delay;
	int y_base;

	BackgroundRockHyperspace();
	void update() override;
	void render() override;
};
