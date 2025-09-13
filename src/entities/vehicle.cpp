#include "entities/vehicle.h"

Vehicle::Vehicle() : hero(NULL), ctrl(NULL), can_be_used(true) {}

void Vehicle::update() {
	if (hero == NULL) {
		if (can_be_used) hero = scanne();

		if (hero != NULL) {
			hero->lockVehicule(canFire(), canChangeDir());
			ctrl = hero->ctrl;
		} else {
			updateNotUsed();
			updateDestroyed();
		}
	} else {
		if (hero->status != ETAT_LOCKEDV || hero->destroyed) {
			hero = NULL;
			updateNotUsed();
			return;
		}

		updateUsed();
	}
}

Ball* Vehicle::scanne() {
	int ddx;
	int ddy;

	for (Ball* c : list_players) {
		ddx = c->x - x;
		ddy = c->y - y;

		if (ddx > -20 && ddx < 20 && ddy > -20 && ddy < 20 && c->status != ETAT_MEURE && c->status != STATE_COME_BACK && !c->destroyed &&
			c->status != ETAT_LOCKEDV)
			return c;
	}

	return NULL;
}
