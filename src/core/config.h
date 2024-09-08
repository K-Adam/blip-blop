#pragma once

#include "core/hi_scores.h"

extern bool vSyncOn;
extern bool fullscreen;

#define LANG_FR 0
#define LANG_UK 1

extern int lang_type;

extern bool music_on;
extern bool sound_on;

extern bool cheat_on;

extern HiScores hi_scores;

#define CONFIG_FILE "config/settings.json"
#define HISCORES_FILE "config/hiscores.json"

void restore_default_saves();
void save_settings();
void load_BB3_config(const char* cfg_file);
void save_BB3_config(const char* cfg_file);
void set_default_config(bool reset_lang = false);
