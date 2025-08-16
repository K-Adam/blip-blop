#include "menus/title_screen.h"
#include "core/config.h"
#include "engine/control_alias.h"
#include "level/world.h"
#include "core/assets.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"
#include "core/text_data.h"

#define NB_TXT 10

void TitleScreen::start() {
	start_music_on = music_on;
	start_sound_on = sound_on;
}

int TitleScreen::update() {
	MenuType next = MenuType(active_menu_->ProcessEvent());

	switch (next) {
		case MenuType::Main:
			active_menu_ = &first_menu_;
			break;
		case MenuType::Options:
			active_menu_ = &options_menu_;
			break;
		case MenuType::Play:
			return RET_START_GAME;
		case MenuType::Exit:
			return RET_EXIT;
	}

	return RET_CONTINUE;
}

void TitleScreen::stop() {
	if (sound_on != start_sound_on) {
		if (sound_on) {
			sbk_bb.loadSFX("bb.sfx");
		}
	}

	if (music_on != start_music_on) {
		if (music_on) {
			if (!current_mbk.empty()) {
				if (!mbk_niveau.open(current_mbk.c_str()))
					music_on = false;
				else {
					if (current_zik >= 0) {
						mbk_niveau.play(current_zik);
					}
				}
			}
		} else {
			mbk_niveau.stop();
		}
	}
}

void TitleScreen::draw(SDL::Surface* surf) { active_menu_->Draw(surf); }
