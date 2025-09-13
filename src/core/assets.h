#pragma once

#include <string>

#include "engine/font.h"
#include "engine/music_bank.h"
#include "engine/picture_bank.h"
#include "engine/sound_bank_bb.h"

extern PictureBank pbk_decor;
extern PictureBank pbk_blip;
extern PictureBank pbk_blop;
extern PictureBank pbk_bb;
extern PictureBank pbk_misc;
extern PictureBank pbk_enemies;
extern PictureBank pbk_level;
extern PictureBank pbk_rpg;
extern PictureBank pbk_rpg_bb;
extern PictureBank pbk_inter;
extern SoundBankBB sbk_bb;
extern SoundBankBB sbk_misc;
extern SoundBankBB sbk_level;
extern MusicBank mbk_level;
extern MusicBank mbk_inter;
extern MusicBank mbk_interl;
extern std::string current_mbk;
extern int current_track;
extern Font fnt_score_blip;
extern Font fnt_score_blop;
extern Font fnt_ammo;
extern Font fnt_ammo_used;
extern Font fnt_cool;
extern Font fnt_rpg;
extern Font fnt_menu;
extern Font fnt_menus;
