#pragma once

#include "core/assets.h"
#include "core/config.h"
#include "core/hi_scores.h"
#include "core/text_data.h"

// Services that live as long as the program
struct App {
	Settings settings;
	HiScores hi_scores;
	TextData texts;
	Assets assets;

	// False when the texts of the chosen language are missing
	bool load();
	void save() const;

	static App& current() {
		static App app;
		return app;
	}
};

inline Settings& settings() { return App::current().settings; }
inline HiScores& hiScores() { return App::current().hi_scores; }
inline const TextData& texts() { return App::current().texts; }
