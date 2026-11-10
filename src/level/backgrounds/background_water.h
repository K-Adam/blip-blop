#pragma once

#include "entities/sprite.h"

// All tiles share one frame counter, alternate tiles run half a cycle apart
struct WaterClock {
	int frame = 0;
};

class BackgroundWaterTile : public Sprite {
   public:
	explicit BackgroundWaterTile(int phase) : phase_(phase) {}

	void update() override {
		pic = assets().pics.level.anim("water")[(level().shared<WaterClock>().frame + phase_) % 10];

		if (x < level().camera.offset - 400) destroyed = true;
	};

   private:
	int phase_;
};

class BackgroundWater : public Sprite {
   public:
	BackgroundWater() {
		bool b = false;

		for (int i = 0; i < ((((int)level().terrain.width) / assets().pics.level.anim("water")[0]->xSize()) + 1); i++) {
			Sprite* ptr = new BackgroundWaterTile(b ? 0 : 5);

			ptr->x = x;
			ptr->y = 454;

			x += 376;
			b = !b;

			level().entities.animated_bgs.emplace_back(ptr);
		}
	};

	void update() override {
		anim_tick += 1;
		anim_tick %= 7;

		if (anim_tick == 0) {
			int& frame = level().shared<WaterClock>().frame;
			frame = (frame + 1) % 10;
		}
	};
};
