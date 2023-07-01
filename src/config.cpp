/******************************************************************
 *
 *
 *		--------------
 *		  Config.cpp
 *		--------------
 *
 *		Contient toutes les données sur la config
 *		et quelques fonctions pour la gérer.
 *
 *
 *		Prosper / LOADED -   V 0.2
 *
 *
 *
 ******************************************************************/

#include <stdio.h>
#include "dd_gfx.h"
#include "ben_debug.h"
#include "config.h"
#include "input.h"
#include "control_alias.h"
#include "fmod.h"
#include "globals.h"
#include "json.h"

using json = nlohmann::json;

bool vSyncOn = true;

int mem_flag = DDSURF_BEST;
bool video_buffer_on = true;
bool mustFixGforceBug = false;

int lang_type = LANG_UK;

bool music_on = true;
bool sound_on = true;

bool cheat_on = false;

HiScores hi_scores;

bool winSet;
bool fullscreen = false;  // THIS IS UGLY AS FUCK. WAY TOO MANY GLOBALS

void load_BB3_config(const char* cfg_file) {
	std::ifstream input(cfg_file);
	if (!input.good()) {
		set_default_config(true);
		return;
	}

	json data = json::parse(input);

	vSyncOn = data["vsync"];
	fullscreen = data["fullscreen"];
	lang_type = data["lang_type"];

	in.setAlias(ALIAS_P1_UP, data["player1"]["UP"]);
	in.setAlias(ALIAS_P1_DOWN, data["player1"]["DOWN"]);
	in.setAlias(ALIAS_P1_LEFT, data["player1"]["LEFT"]);
	in.setAlias(ALIAS_P1_RIGHT, data["player1"]["RIGHT"]);
	in.setAlias(ALIAS_P1_FIRE, data["player1"]["FIRE"]);
	in.setAlias(ALIAS_P1_JUMP, data["player1"]["JUMP"]);
	in.setAlias(ALIAS_P1_SUPER, data["player1"]["SUPER"]);

	in.setAlias(ALIAS_P2_UP, data["player2"]["UP"]);
	in.setAlias(ALIAS_P2_DOWN, data["player2"]["DOWN"]);
	in.setAlias(ALIAS_P2_LEFT, data["player2"]["LEFT"]);
	in.setAlias(ALIAS_P2_RIGHT, data["player2"]["RIGHT"]);
	in.setAlias(ALIAS_P2_FIRE, data["player2"]["FIRE"]);
	in.setAlias(ALIAS_P2_JUMP, data["player2"]["JUMP"]);
	in.setAlias(ALIAS_P2_SUPER, data["player2"]["SUPER"]);

	// ?
	lang_type = LANG_UK;
}

void save_BB3_config(const char* cfg_file) {
	json data;

	data["vsync"] = vSyncOn;
	data["fullscreen"] = fullscreen;
	data["lang_type"] = lang_type;

	data["player1"] = json::object();
	data["player2"] = json::object();

	data["player1"]["UP"] = in.getAlias(ALIAS_P1_UP);
	data["player1"]["DOWN"] = in.getAlias(ALIAS_P1_DOWN);
	data["player1"]["LEFT"] = in.getAlias(ALIAS_P1_LEFT);
	data["player1"]["RIGHT"] = in.getAlias(ALIAS_P1_RIGHT);
	data["player1"]["FIRE"] = in.getAlias(ALIAS_P1_FIRE);
	data["player1"]["JUMP"] = in.getAlias(ALIAS_P1_JUMP);
	data["player1"]["SUPER"] = in.getAlias(ALIAS_P1_SUPER);

	data["player2"]["UP"] = in.getAlias(ALIAS_P2_UP);
	data["player2"]["DOWN"] = in.getAlias(ALIAS_P2_DOWN);
	data["player2"]["LEFT"] = in.getAlias(ALIAS_P2_LEFT);
	data["player2"]["RIGHT"] = in.getAlias(ALIAS_P2_RIGHT);
	data["player2"]["FIRE"] = in.getAlias(ALIAS_P2_FIRE);
	data["player2"]["JUMP"] = in.getAlias(ALIAS_P2_JUMP);
	data["player2"]["SUPER"] = in.getAlias(ALIAS_P2_SUPER);

	std::ofstream out(cfg_file);
	out << data.dump(2);
}

void set_default_config(bool reset_lang) {
	if (reset_lang) lang_type = LANG_UK;

	in.setAlias(ALIAS_P1_UP, DIK_UP);
	in.setAlias(ALIAS_P1_DOWN, DIK_DOWN);
	in.setAlias(ALIAS_P1_LEFT, DIK_LEFT);
	in.setAlias(ALIAS_P1_RIGHT, DIK_RIGHT);
	in.setAlias(ALIAS_P1_FIRE, DIK_LCONTROL);
	in.setAlias(ALIAS_P1_JUMP, DIK_LMENU);
	in.setAlias(ALIAS_P1_SUPER, DIK_BACKSPACE);

	in.setAlias(ALIAS_P2_UP, DIK_W);
	in.setAlias(ALIAS_P2_DOWN, DIK_A);
	in.setAlias(ALIAS_P2_LEFT, DIK_A);
	in.setAlias(ALIAS_P2_RIGHT, DIK_D);
	in.setAlias(ALIAS_P2_FIRE, DIK_TAB);
	in.setAlias(ALIAS_P2_JUMP, DIK_SPACE);
	in.setAlias(ALIAS_P2_SUPER, DIK_F);
}
