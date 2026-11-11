#include "entities/vehicle.h"

Vehicle::Vehicle() : can_be_used(true), hero(nullptr), ctrl(nullptr) {}

void Vehicle::update() {
	if (hero == nullptr) {
		if (can_be_used) hero = seekRider();

		if (hero != nullptr) {
			hero->lockVehicle(canFire(), canChangeDir());
			ctrl = hero->ctrl;
		} else {
			updateNotUsed();
			updateDestroyed();
		}
	} else {
		if (hero->status != STATE_LOCKED_VEHICLE || hero->destroyed) {
			hero = nullptr;
			updateNotUsed();
			return;
		}

		updateUsed();
	}
}

Ball* Vehicle::seekRider() {
	int ddx;
	int ddy;

	for (Ball* c : session().players) {
		ddx = c->x - x;
		ddy = c->y - y;

		if (ddx > -20 && ddx < 20 && ddy > -20 && ddy < 20 && c->status != STATE_DYING && c->status != STATE_COME_BACK && !c->destroyed &&
			c->status != STATE_LOCKED_VEHICLE)
			return c;
	}

	return nullptr;
}
