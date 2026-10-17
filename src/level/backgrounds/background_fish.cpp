#include "entities/sprite.h"
#include "level/backgrounds/background_fish.h"

BackgroundFishRed::BackgroundFishRed() { dir = rand() % 2; }

void BackgroundFishRed::update() {
	anim_tick++;
	anim_tick %= 9;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dir) {
		x += FISH_SPEED;
		pic = pbk_level.anim("fish_red_right")[anim_step];
	} else {
		x -= FISH_SPEED;
		pic = pbk_level.anim("fish_red_left")[anim_step];
	}

	if ((x < offset - 100) || (x > offset + 740)) {
		destroyed = true;
	}
}

BackgroundFishPurple::BackgroundFishPurple() { dir = rand() % 2; }

void BackgroundFishPurple::update() {
	anim_tick++;
	anim_tick %= 9;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dir) {
		x += FISH_SPEED;
		pic = pbk_level.anim("fish_purple_right")[anim_step];
	} else {
		x -= FISH_SPEED;
		pic = pbk_level.anim("fish_purple_left")[anim_step];
	}

	if ((x < offset - 100) || (x > offset + 740)) {
		destroyed = true;
	}
};

BackgroundFishRedCamouflaged::BackgroundFishRedCamouflaged() { dir = rand() % 2; }

void BackgroundFishRedCamouflaged::update() {
	anim_tick++;
	anim_tick %= 9;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dir) {
		x += FISH_SPEED;
		pic = pbk_level.anim("fish_red_camouflaged_right")[anim_step];
	} else {
		x -= FISH_SPEED;
		pic = pbk_level.anim("fish_red_camouflaged_left")[anim_step];
	}

	if ((x < offset - 100) || (x > offset + 740)) {
		destroyed = true;
	}
}

BackgroundFishPurpleCamouflaged::BackgroundFishPurpleCamouflaged() { dir = rand() % 2; }

void BackgroundFishPurpleCamouflaged::update() {
	anim_tick++;
	anim_tick %= 9;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dir) {
		x += FISH_SPEED;
		pic = pbk_level.anim("fish_purple_camouflaged_right")[anim_step];
	} else {
		x -= FISH_SPEED;
		pic = pbk_level.anim("fish_purple_camouflaged_left")[anim_step];
	}

	if ((x < offset - 100) || (x > offset + 740)) {
		destroyed = true;
	}
}
