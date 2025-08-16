#pragma once

#include "entities/shots/shot.h"

class ShotGift : public Shot {
   public:
	ShotGift();
	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
