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

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
