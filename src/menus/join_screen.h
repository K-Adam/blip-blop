#pragma once

#include "engine/scheme_controller.h"

// Each player joins with the device they hold, and the scheme they used drives them in game
class JoinScreen {
   public:
	enum class Output { Continue, Back, Start };

	Output update();
	void draw() const;

	int players() const { return players_; }
	const SchemeController& player(int i) const { return joined_[i]; }

   private:
	// True when this input starts the game, after its first press joined it
	bool press(int scheme, int pad);

	SchemeController joined_[2];
	int players_ = 0;
};
