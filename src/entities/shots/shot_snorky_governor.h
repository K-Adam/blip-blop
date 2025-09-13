#pragma once

#include "entities/shots/shot.h"
#include "entities/character.h"

#define SHOT_INERTIA 6
#define SPEED_MAX 6
#define TIME_MAX 100

class ShotSnorkyGovernor : public Shot {
   public:
	int dx;
	int dx_tick;
	int dy_tick;
	int time;
	Character* target_player;

	ShotSnorkyGovernor(Character* target, int vx, int vy);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
