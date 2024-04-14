#pragma once

#include "entities/shots/shot.h"

class ShotBisouHeart : public Shot {
   public:
	int xbase;
	int phi;
	bool ok;

	ShotBisouHeart();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
