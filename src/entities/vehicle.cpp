#include "entities/vehicle.h"

Vehicle::Vehicle() : joueur(NULL), ctrl(NULL), can_be_used(true) {}

void Vehicle::update() {
	if (joueur == NULL) {
		if (can_be_used) joueur = scanne();

		if (joueur != NULL) {
			joueur->lockVehicule(canFire(), canChangeDir());
			ctrl = joueur->ctrl;
		} else {
			updateNotUsed();
			updateADetruire();
		}
	} else {
		if (joueur->etat != ETAT_LOCKEDV || joueur->a_detruire) {
			joueur = NULL;
			updateNotUsed();
			return;
		}

		updateUsed();
	}
}

Ball* Vehicle::scanne() {
	int ddx;
	int ddy;

	for (Ball* c : list_joueurs) {
		ddx = c->x - x;
		ddy = c->y - y;

		if (ddx > -20 && ddx < 20 && ddy > -20 && ddy < 20 && c->etat != ETAT_MEURE && c->etat != ETAT_COME_BACK && !c->a_detruire && c->etat != ETAT_LOCKEDV)
			return c;
	}

	return NULL;
}
