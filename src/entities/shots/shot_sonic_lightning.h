#pragma once

#include "entities/shots/shot.h"

class ShotSonicLightning : public Shot {
   public:
	ShotSonicLightning();

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
	void affiche() override;
};
