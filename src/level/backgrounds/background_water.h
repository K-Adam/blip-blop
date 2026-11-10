#pragma once

#include "entities/sprite.h"

// All tiles share one frame counter, alternate tiles run half a cycle apart
inline int water_frame = 0;

class BackgroundWaterTile : public Sprite {
   public:
	explicit BackgroundWaterTile(int phase) : phase_(phase) {}

	void update() override {
		pic = pbk_level.anim("water")[(water_frame + phase_) % 10];

		if (x < offset - 400) destroyed = true;
	};

   private:
	int phase_;
};

class BackgroundWater : public Sprite {
   public:
	BackgroundWater() {
		bool b = false;

		for (int i = 0; i < ((((int)level_size) / pbk_level.anim("water")[0]->xSize()) + 1); i++) {
			Sprite* ptr = new BackgroundWaterTile(b ? 0 : 5);

			ptr->x = x;
			ptr->y = 454;

			x += 376;
			b = !b;

			list_animated_bgs.emplace_back(ptr);
		}
	};

	void update() override {
		anim_tick += 1;
		anim_tick %= 7;

		if (anim_tick == 0) water_frame = (water_frame + 1) % 10;
	};
};
