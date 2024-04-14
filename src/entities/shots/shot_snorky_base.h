#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 20

class ShotSnorkyBase : public Shot {
   public:
	int oscilation_etape;
	int speed_etape;
	int dx;

	ShotSnorkyBase(int vx);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
