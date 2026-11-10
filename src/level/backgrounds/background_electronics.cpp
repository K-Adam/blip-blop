#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_electronics.h"

void BackgroundElectronic1::update() {
	step(2, 15);
	pic = pbk_level.anim("electronic_board")[anim_step];
}

void BackgroundElectronic1::render() {
	Sprite::render();
	draw(x, y, pbk_level.pic("electronic1_overlay"));
}

void BackgroundElectronic2::update() {
	step(2, 15);
	pic = pbk_level.anim("electronic_board")[anim_step];
}

void BackgroundElectronic2::render() {
	Sprite::render();
	draw(x, y, pbk_level.pic("electronic2_overlay"));
}

void BackgroundElectronic3::update() {
	step(2, 15);
	pic = pbk_level.anim("electronic_board")[anim_step];
}

void BackgroundElectronic3::render() {
	Sprite::render();
	draw(x, y, pbk_level.pic("electronic3_overlay"));
}

void BackgroundElectronic4::update() {
	step(2, 15);
	pic = pbk_level.anim("electronic_board")[anim_step];
}
