#pragma once

#include "fond_anime.h"

class FondEcrancamera : public FondAnime {
   public:
	FondEcrancamera() : FondAnime(50, 3, 87, false) {}
	void affiche() override;
};
