#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_snork_airlock.h"
#include "entities/enemies/enemy.h"

int BackgroundSnorkAirlock1::status = 0;  // 1 once airlock 1 is closed

BackgroundSnorkAirlock1::BackgroundSnorkAirlock1() : players_inside(0) { anim_step = 0; }

void BackgroundSnorkAirlock1::update() {
	if (!status) {
		if ((targeted_player != NULL) && (targeted_player->x > 1400)) {
			players_inside++;
			if (anim_step < 222) {
				if (players_inside >= 2) {
					anim_step += 2;
				}
			} else {
				status = 1;
			}
		} else {
			players_inside = 0;
		}

		if (anim_step % 8 == 0) {
			solid_walls[(y + anim_step) / 8][(x + 8) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 16) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 24) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 32) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 40) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 48) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 56) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 64) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 72) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 80) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 88) / 8] = true;
			solid_walls[(y + anim_step) / 8][(x + 96) / 8] = true;
		}
	}
}

void BackgroundSnorkAirlock1::render() {
	SDL::Surface* surf;
	int xs;
	int ys;
	Rect r;
	int width_px;
	int shift = 0;

	surf = pbk_level[1]->Surf();
	xs = pbk_level[1]->xSize();
	ys = pbk_level[1]->ySize();

	r.top = 222 - anim_step;
	r.left = 0;

	if (x + 53 > offset + 640)
		width_px = offset + 640 - x;
	else if (x < offset) {
		width_px = 53;
		r.left = offset - x;
		shift = offset - x;
	} else
		width_px = 53;

	r.right = width_px;
	r.bottom = 222;

	backSurface->BltFast(x - offset + shift, y, surf, &r);
}

BackgroundSnorkAirlock2::BackgroundSnorkAirlock2() { anim_step = 222; }

void BackgroundSnorkAirlock2::update() {
	if ((BackgroundSnorkAirlock1::status) && (anim_step > 0)) {
		anim_step--;
	}

	if (anim_step % 8 == 0) {
		solid_walls[(y + anim_step) / 8][(x + 8) / 8] = false;
		solid_walls[(y + anim_step) / 8][(x + 16) / 8] = false;
		solid_walls[(y + anim_step) / 8][(x + 24) / 8] = false;
		solid_walls[(y + anim_step) / 8][(x + 32) / 8] = false;
		solid_walls[(y + anim_step) / 8][(x + 40) / 8] = false;
		solid_walls[(y + anim_step) / 8][(x + 48) / 8] = false;
	}
}

void BackgroundSnorkAirlock2::render() {
	SDL::Surface* surf;
	int xs;
	int ys;
	Rect r;
	int width_px;
	int shift = 0;

	surf = pbk_level[1]->Surf();
	xs = pbk_level[1]->xSize();
	ys = pbk_level[1]->ySize();

	r.top = 222 - anim_step;
	r.left = 0;

	if (x + 53 > offset + 640)
		width_px = offset + 640 - x;
	else if (x < offset) {
		width_px = 53;
		r.left = offset - x;
		shift = offset - x;
	} else
		width_px = 53;

	r.right = width_px;
	r.bottom = 222;

	backSurface->BltFast(x - offset + shift, y, surf, &r);
}
