#pragma once

#include "entities/sprite.h"

class BackgroundBomb : public Sprite {
   private:
	int etape_compteur;

	int seconde;
	int centieme;

	int armed;

	int etape_warning;
	int ss_etape_warning;

	int etape_nuke;
	int ss_etape_nuke;

   public:
	BackgroundBomb();

	void update() override;
	void affiche() override;
};
