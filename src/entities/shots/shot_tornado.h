#pragma once

#include "entities/shots/shot.h"

#define SPEED_MAX 7
#define TORNADE_ACCELERATION 10
#define TOUR_MAX 1
#define LATENCE_TORNADE 21

class ShotTornado : public Shot {
   public:
	int dx;
	int etape_speed;
	int nb_tour;

	ShotTornado(int vx);

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
