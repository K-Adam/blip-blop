#include "entities/sprite.h"
#include "level/backgrounds/background_poke_switch.h"
#include "entities/enemies/enemy.h"
#include "entities/cool_text.h"

const int anim_switch_on[] = {0, 1, 2, 0};
const int anim_switch_off[] = {0, 1, 2, 1};

BackgroundPokeSwitch::BackgroundPokeSwitch() : status(false), wait_msg(false), elevator_msg(false), bravo(false) {}

void BackgroundPokeSwitch::update() {
	anim_tick++;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (status) {
		pic = assets().pics.level.anim("switch_on")[anim_switch_on[anim_step]];
	} else {
		if (session().targeted_player != nullptr) {
			if ((session().targeted_player->x > x - 10) && (session().targeted_player->x < x + 33) && (session().targeted_player->y > y - 10) &&
				(session().targeted_player->y < y + 51)) {
				level().rules.flags[2]++;
				status = true;
				assets().sounds.level.play(2);
			}
		}
		pic = assets().pics.level.anim("switch_off")[anim_switch_off[anim_step]];
	}

	if (x < level().camera.offset - 100) destroyed = true;

	if (level().rules.flags[0] == 0) {
		if (wait_msg || level().rules.flags[2] == 0) {
			level().rules.flags[7] = 1;
		} else {
			level().rules.flags[7] = 0;
		}
	}

	if (!wait_msg && level().rules.flags[2] == 0 && level().rules.flags[6] <= 80) {
		wait_msg = true;
		auto txt = std::make_unique<CoolText>();
		txt->ntxt = 104;
		level().entities.cool_texts.emplace_back(std::move(txt));
	}

	if (!elevator_msg && wait_msg && level().rules.flags[2] == 1) {
		elevator_msg = true;
		auto txt = std::make_unique<CoolText>();
		txt->ntxt = 105;
		level().entities.cool_texts.emplace_back(std::move(txt));
		txt = std::make_unique<CoolText>();
		txt->ntxt = 106;
		level().entities.cool_texts.emplace_back(std::move(txt));
	}

	if (!bravo && elevator_msg && level().rules.flags[0] == 1) {
		bravo = true;
		auto txt = std::make_unique<CoolText>();
		txt->ntxt = 107;
		level().entities.cool_texts.emplace_back(std::move(txt));
	}
};
