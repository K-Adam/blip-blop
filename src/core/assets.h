#pragma once

#include "engine/font.h"
#include "engine/music_bank.h"
#include "engine/picture_bank.h"
#include "engine/sound_bank.h"

// Banks loaded at startup or per level, owned by App
struct Assets {
	struct {
		PictureBank decor;
		PictureBank blip;
		PictureBank blop;
		PictureBank bb;
		PictureBank misc;
		PictureBank enemies;
		PictureBank level;
		PictureBank rpg;
		PictureBank rpg_bb;
		PictureBank inter;
	} pics;

	struct {
		SoundBank bb;
		SoundBank misc;
		SoundBank level;
	} sounds;

	struct {
		MusicBank level;
		MusicBank inter;
		MusicBank interl;
	} music;

	struct {
		Font score_blip;
		Font score_blop;
		Font ammo;
		Font ammo_used;
		Font cool;
		Font rpg;
		Font menu;
		Font menus;
	} fonts;
};

Assets& assets();
