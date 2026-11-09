#include "entities/shots/shot_bb_shotgun.h"

#include <cmath>
#include <numbers>

const int delay_big[] = {1, 2, 2, 1, 2, 1};
const int d_big[] = {3, -2, -1, 0, 1, 2, 3};
const int d_small[] = {-2, 0, 2, 0, 1, -1, 2, -1, -2, 1};

// Shared across pellets so one blast spreads instead of firing 17 identical pellets
int n_delay_big = 0;
int n_d_big = 0;
int n_d_small = 0;

void ShotBBShotgun::aim(double deg) {
	n_delay_big = (n_delay_big + 1) % 6;
	n_d_big = (n_d_big + 1) % 7;
	n_d_small = (n_d_small + 1) % 10;

	// Each pellet jitters along the aim and slowly drifts across it
	setAngle(deg, 15 + d_big[n_d_big]);
	double a = deg * std::numbers::pi / 180;
	drift_x_ = std::sin(a) * d_small[n_d_small];
	drift_y_ = std::cos(a) * d_small[n_d_small];
	drift_delay_ = delay_big[n_delay_big];
	drift_tick_ = 0;

	lifespan = 10 + rand() % 7;
	age = 0;
}

void ShotBBShotgun::update() {
	drift_tick_ = (drift_tick_ + 1) % drift_delay_;
	if (drift_tick_ == 0) push(drift_x_, drift_y_);
	moveAlongAngle();

	age += 1;

	if (solid_wall(x, y) || x < offset - 20 || x > offset + 660 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		if (age < lifespan)
			pic = pbk_bb.pic("shot_pellet");
		else
			pic = pbk_bb.pic("shot_m16");

		colAlongAngle();
	}
}
