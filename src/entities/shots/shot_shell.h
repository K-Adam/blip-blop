#pragma once

#include "entities/shots/shot.h"

#define CARAPACE_SPEED 6

class ShotShell : public Shot {
   public:
	int dx;
	bool rebond;

	ShotShell();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
