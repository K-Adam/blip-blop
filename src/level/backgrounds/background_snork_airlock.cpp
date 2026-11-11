#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_snork_airlock.h"
#include "entities/enemies/enemy.h"

namespace {
// Airlock 2 opens once airlock 1 has closed behind the players
struct AirlockState {
	bool first_closed = false;
};
}  // namespace

BackgroundSnorkAirlock1::BackgroundSnorkAirlock1() : players_inside(0) { anim_step = 0; }

void BackgroundSnorkAirlock1::update() {
	bool& first_closed = level().shared<AirlockState>().first_closed;
	if (!first_closed) {
		if ((session().targeted_player != nullptr) && (session().targeted_player->x > 1400)) {
			players_inside++;
			if (anim_step < 222) {
				if (players_inside >= 2) {
					anim_step += 2;
				}
			} else {
				first_closed = true;
			}
		} else {
			players_inside = 0;
		}

		if (anim_step % 8 == 0) {
			level().terrain.solid[(y + anim_step) / 8][(x + 8) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 16) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 24) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 32) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 40) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 48) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 56) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 64) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 72) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 80) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 88) / 8] = true;
			level().terrain.solid[(y + anim_step) / 8][(x + 96) / 8] = true;
		}
	}
}

void BackgroundSnorkAirlock1::render() {
	Rect r;
	int width_px;
	int pic_offset = 0;

	r.top = 222 - anim_step;
	r.left = 0;

	if (x + 53 > level().camera.offset + 640)
		width_px = level().camera.offset + 640 - x;
	else if (x < level().camera.offset) {
		width_px = 53;
		r.left = level().camera.offset - x;
		pic_offset = level().camera.offset - x;
	} else
		width_px = 53;

	r.right = width_px;
	r.bottom = 222;

	assets().pics.level.pic("airlock_door")->BlitPart(graphics.back(), x - level().camera.offset + pic_offset, y, r);
}

BackgroundSnorkAirlock2::BackgroundSnorkAirlock2() { anim_step = 222; }

void BackgroundSnorkAirlock2::update() {
	if (level().shared<AirlockState>().first_closed && anim_step > 0) {
		anim_step--;
	}

	if (anim_step % 8 == 0) {
		level().terrain.solid[(y + anim_step) / 8][(x + 8) / 8] = false;
		level().terrain.solid[(y + anim_step) / 8][(x + 16) / 8] = false;
		level().terrain.solid[(y + anim_step) / 8][(x + 24) / 8] = false;
		level().terrain.solid[(y + anim_step) / 8][(x + 32) / 8] = false;
		level().terrain.solid[(y + anim_step) / 8][(x + 40) / 8] = false;
		level().terrain.solid[(y + anim_step) / 8][(x + 48) / 8] = false;
	}
}

void BackgroundSnorkAirlock2::render() {
	Rect r;
	int width_px;
	int pic_offset = 0;

	r.top = 222 - anim_step;
	r.left = 0;

	if (x + 53 > level().camera.offset + 640)
		width_px = level().camera.offset + 640 - x;
	else if (x < level().camera.offset) {
		width_px = 53;
		r.left = level().camera.offset - x;
		pic_offset = level().camera.offset - x;
	} else
		width_px = 53;

	r.right = width_px;
	r.bottom = 222;

	assets().pics.level.pic("airlock_door")->BlitPart(graphics.back(), x - level().camera.offset + pic_offset, y, r);
}
