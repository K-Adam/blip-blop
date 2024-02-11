#pragma once

#include "tir.h"

#define DELAY_ACCELERATION 8

class TirMarioFireball : public Tir {
   public:
	int speed_etape;
	int dx;

	TirMarioFireball(int vx);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
