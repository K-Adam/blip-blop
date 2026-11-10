#include "level/backgrounds/background_rock_hyperspace.h"
#include <cmath>
#include <numbers>

BackgroundRockHyperspace::BackgroundRockHyperspace() : placed(1), delta(rand() % 360), sinusoid_delay(1), y_base(0) {}

void BackgroundRockHyperspace::update() {
	delta += sinusoid_delay;
	delta %= 360;

	if (placed) {
		if ((x >= 0) && (x < 200)) {
			x_delay = 2 + rand() % 3;
			rock_layer = 2;
			pic = assets().pics.level.anim("rock_hyperspace_far")[rand() % 7];
			sinusoid_delay = 2 + rand() % 2;
		} else if ((x >= 200) && (x < 420)) {
			x_delay = 4 + rand() % 5;
			rock_layer = 1;
			pic = assets().pics.level.anim("rock_hyperspace_mid")[rand() % 5];
			sinusoid_delay = 3 + rand() % 2;
		} else if ((x >= 420) && (x < 640)) {
			x_delay = 8 + rand() % 4;
			rock_layer = 0;
			pic = assets().pics.level.anim("rock_hyperspace_near")[rand() % 5];
			sinusoid_delay = 4 + rand() % 2;
		}
		x = rand() % 640;
		y_base = rand() % 380;
		y = rand() % 380;
		placed = 0;
	}
	anim_tick++;
	anim_tick %= x_delay;
	x += x_delay;

	switch (rock_layer) {
		case 0:
			y = y_base + int(11 * std::cos(delta * std::numbers::pi / 180));
			break;

		case 1:
			y = y_base + int(8 * std::cos(delta * std::numbers::pi / 180));
			break;

		case 2:
			y = y_base + int(8 * std::cos(delta * std::numbers::pi / 180));
			break;
	}

	if (x > level().camera.offset + 700) {
		x = level().camera.offset - 80;
		y_base = rand() % 380;
		y = rand() % 380;
		switch (rock_layer) {
			case 0:
				x_delay = 8 + rand() % 4;
				pic = assets().pics.level.anim("rock_hyperspace_near")[rand() % 5];
				sinusoid_delay = 4 + rand() % 2;
				break;

			case 1:
				x_delay = 4 + rand() % 5;
				pic = assets().pics.level.anim("rock_hyperspace_mid")[rand() % 5];
				sinusoid_delay = 3 + rand() % 2;
				break;

			case 2:
				x_delay = 2 + rand() % 3;
				pic = assets().pics.level.anim("rock_hyperspace_far")[rand() % 7];
				sinusoid_delay = 2 + rand() % 2;
				break;
		}
	}
}

void BackgroundRockHyperspace::render() {
	if (!placed) {
		Sprite::render();
	}
}
