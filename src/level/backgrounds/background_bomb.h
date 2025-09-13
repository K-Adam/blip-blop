#pragma once

#include "entities/sprite.h"

class BackgroundBomb : public Sprite {
   private:
	int counter_step;

	int seconds;
	int hundredths;

	int armed;

	int warning_step;
	int warning_tick;

	int nuke_step;
	int nuke_tick;

   public:
	BackgroundBomb();

	void update() override;
	void render() override;
};
