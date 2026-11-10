#pragma once

class Input;

#define LANG_FR 0
#define LANG_UK 1

#define CONFIG_FILE "config/settings.json"
#define HISCORES_FILE "config/hiscores.json"

// The player's preferences, the settings file also stores the key bindings held by Input
struct Settings {
	bool vsync = true;
	bool fullscreen = false;
	int lang = LANG_UK;
	bool cheat = false;

	void load(const char* file, Input& input);
	void save(const char* file, const Input& input) const;
	void setDefaults(Input& input, bool reset_lang = false);
};
