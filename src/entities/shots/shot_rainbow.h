#pragma once

class ShotRainbow : public Shot {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) anim_step += 1;

		if (anim_step >= 15) {
			destroyed = true;
		} else {
			pic = facing(assets().pics.enemies, "sky_rainbow")[anim_step];

			colFromPic();
		}
	};
};
