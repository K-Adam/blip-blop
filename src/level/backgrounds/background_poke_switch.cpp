#include "entities/sprite.h"
#include "level/backgrounds/background_poke_switch.h"
#include "entities/enemies/enemy.h"
#include "entities/cool_text.h"

const int anim_interupteur_on[] = {84, 85, 86, 84};
const int anim_interupteur_off[] = {81, 82, 83, 82};

BackgroundPokeSwitch::BackgroundPokeSwitch() : status(false), wait_msg(false), ascenc_msg(false), bravo(false) {}

void BackgroundPokeSwitch::update() {
	anim_tick++;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (status) {
		pic = pbk_level[anim_interupteur_on[anim_step]];
	} else {
		if (targeted_player != NULL) {
			if ((targeted_player->x > x - 10) && (targeted_player->x < x + 33) && (targeted_player->y > y - 10) && (targeted_player->y < y + 51)) {
				game_flag[2]++;
				status = true;
				sbk_level.play(2);
			}
		}
		pic = pbk_level[anim_interupteur_off[anim_step]];
	}

	if (x < offset - 100) destroyed = true;

	if (game_flag[0] == 0) {
		if (wait_msg || game_flag[2] == 0) {
			game_flag[7] = 1;
		} else {
			game_flag[7] = 0;
		}
	}

	if (!wait_msg && game_flag[2] == 0 && game_flag[6] <= 80) {
		wait_msg = true;
		CoolText* txt = new CoolText();
		txt->ntxt = 104;
		list_txt_cool.emplace_back(txt);
	}

	if (!ascenc_msg && wait_msg && game_flag[2] == 1) {
		ascenc_msg = true;
		CoolText* txt = new CoolText();
		txt->ntxt = 105;
		list_txt_cool.emplace_back(txt);
		txt = new CoolText();
		txt->ntxt = 106;
		list_txt_cool.emplace_back(txt);
	}

	if (!bravo && ascenc_msg && game_flag[0] == 1) {
		bravo = true;
		CoolText* txt = new CoolText();
		txt->ntxt = 107;
		list_txt_cool.emplace_back(txt);
	}
};
