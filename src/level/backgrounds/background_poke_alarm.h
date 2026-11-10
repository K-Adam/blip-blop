#pragma once

#include "entities/sprite.h"

class BackgroundPokeAlarm1 : public Sprite {
   public:
	void update() override {
		step(3, 14);

		pic = pbk_level.anim("alarm_1")[anim_step];
		updateDestroyed();
	};
};

class BackgroundPokeAlarm2 : public Sprite {
   public:
	void update() override {
		step(3, 14);

		pic = pbk_level.anim("alarm_2")[anim_step];
		updateDestroyed();
	};
};

class BackgroundPokeAlarm3 : public Sprite {
   public:
	void update() override {
		step(3, 14);

		pic = pbk_level.anim("alarm_3")[anim_step];
		updateDestroyed();
	};
};
