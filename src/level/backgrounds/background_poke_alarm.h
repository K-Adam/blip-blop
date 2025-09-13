#pragma once

#include "entities/sprite.h"

class BackgroundPokeAlarm1 : public Sprite {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 14;
		}

		pic = pbk_level[anim_step];
		updateDestroyed();
	};
};

class BackgroundPokeAlarm2 : public Sprite {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 14;
		}

		pic = pbk_level[14 + anim_step];
		updateDestroyed();
	};
};

class BackgroundPokeAlarm3 : public Sprite {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 14;
		}

		pic = pbk_level[28 + anim_step];
		updateDestroyed();
	};
};
