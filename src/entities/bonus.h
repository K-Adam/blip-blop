#pragma once

#include <cmath>
#include <numbers>
#include "entities/ball.h"
#include "entities/sprite.h"

#define CORNER_DIST 25

class Bonus : public Sprite {
   public:
	int phi;
	bool local_phase;

	Bonus() : phi(0), local_phase(0){};

	virtual void onPickup(Ball* c) {
		destroyed = true;
		col_on = false;
	};

	void update() override {
		fall();

		phi += 12;
		phi %= 360;

		anim_step += 1;
		anim_step %= 10;

		if (anim_step == 0) local_phase = !local_phase;

		if (x < level().camera.offset - 50) destroyed = true;

		y -= 15;
		colFromPic();
		y += 15;
	};

	void render() override {
		y -= 15;
		Sprite::render();

		Flipbook corners = assets().pics.misc.anim(local_phase ? "bonus_corners_on" : "bonus_corners_off");

		int d = int(8 * std::sin(phi * std::numbers::pi / 180));

		draw(x - CORNER_DIST - d, y - CORNER_DIST - d, corners[3]);
		draw(x + CORNER_DIST + d, y - CORNER_DIST - d, corners[2]);
		draw(x + CORNER_DIST + d, y + CORNER_DIST + d, corners[0]);
		draw(x - CORNER_DIST - d, y + CORNER_DIST + d, corners[1]);

		y += 15;
	};

	void colFromPic() override {
		Sprite::colFromPic();

		x1 -= 10;
		y1 -= 10;
		x2 += 10;
		y2 += 10;
	};
};
