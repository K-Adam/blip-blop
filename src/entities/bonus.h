#pragma once

#include <cmath>
#include <numbers>
#include "entities/ball.h"
#include "entities/sprite.h"

#define DIST_BITONIO 25

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

		if (x < offset - 50) destroyed = true;

		y -= 15;
		colFromPic();
		y += 15;
	};

	void render() override {
		int base;

		y -= 15;
		Sprite::render();

		if (local_phase)
			base = 0;
		else
			base = 4;

		int d = int(8 * std::sin(phi * std::numbers::pi / 180));

		draw(x - DIST_BITONIO - d, y - DIST_BITONIO - d, pbk_misc[base + 3]);
		draw(x + DIST_BITONIO + d, y - DIST_BITONIO - d, pbk_misc[base + 2]);
		draw(x + DIST_BITONIO + d, y + DIST_BITONIO + d, pbk_misc[base]);
		draw(x - DIST_BITONIO - d, y + DIST_BITONIO + d, pbk_misc[base + 1]);

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
