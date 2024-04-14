#pragma once

#include "level/backgrounds/background_animated.h"

class FondEcrancamera : public FondAnime {
   public:
	FondEcrancamera() : FondAnime(50, 3, 87, false) {}
	void affiche() override;
};
