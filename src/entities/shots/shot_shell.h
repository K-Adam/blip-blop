#pragma once

#include "entities/shots/shot.h"

#define CARAPACE_SPEED 6

class ShotShell : public Shot {
   public:
	int dx;
	bool rebond;

	ShotShell();

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
