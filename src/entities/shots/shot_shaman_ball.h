#pragma once

#include "entities/shots/shot.h"

class ShotShamanBall : public Shot {
   public:
	int dx;
	bool ok;
	bool boom;

	ShotShamanBall();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
