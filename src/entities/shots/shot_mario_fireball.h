#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 8

class ShotMarioFireball : public Shot {
   public:
	int speed_etape;
	int dx;

	ShotMarioFireball(int vx);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
