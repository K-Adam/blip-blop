#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_electronics.h"

void BackgroundElectronic1::update() {
	anim_tick += 1;
	anim_tick %= 2;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 15;
	}
	pic = pbk_level[27 + anim_step];
}

void BackgroundElectronic1::render() {
	Sprite::render();
	draw(x, y, pbk_level[42]);
}

void BackgroundElectronic2::update() {
	anim_tick += 1;
	anim_tick %= 2;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 15;
	}
	pic = pbk_level[27 + anim_step];
}

void BackgroundElectronic2::render() {
	Sprite::render();
	draw(x, y, pbk_level[44]);
}

void BackgroundElectronic3::update() {
	anim_tick += 1;
	anim_tick %= 2;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 15;
	}
	pic = pbk_level[27 + anim_step];
}

void BackgroundElectronic3::render() {
	Sprite::render();
	draw(x, y, pbk_level[43]);
}

void BackgroundElectronic4::update() {
	anim_tick += 1;
	anim_tick %= 2;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 15;
	}
	pic = pbk_level[27 + anim_step];
}
