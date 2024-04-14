#include "entities/enemies/enemy_princess_guard.h"
#include "entities/enemies/enemy_princess.h"

const int anim_guard_marche_gauche[] = {135, 136, 137, 136};

EnnemiPrincesseguard::EnnemiPrincesseguard(EnnemiPrincesse* Princesse, int distance) : princesse(Princesse), distance_couverture(distance) {
	pv = 2500;
	tresor = 100;
	dir = SENS_GAUCHE;
}

void EnnemiPrincesseguard::update() {
	if (blood > 0) blood -= 1;

	switch (etat) {
		case ETAT_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;
	}

	updateADetruire();
}

void EnnemiPrincesseguard::onAvance() {
	// Si plus de plateformes on passe dans l'etat TOMBE

	// pour marcher
	if (princesse->a_detruire == true) {
		a_detruire = true;
	}

	if (game_flag[2]) {
		etat = ETAT_MEURE;
		onMeure();
		return;
	}
	tombe();
	if (princesse != NULL) {
		if ((x + distance_couverture + GUARD_SPEED) < princesse->x) {
			marche(GUARD_SPEED);
			pic = pbk_ennemis[anime(anim_guard_marche_gauche, 4, 6)];
		} else if ((x + distance_couverture - GUARD_SPEED) > princesse->x) {
			marche(-GUARD_SPEED * 3);
			pic = pbk_ennemis[anime(anim_guard_marche_gauche, 4, 6)];
		} else {
			pic = pbk_ennemis[135];
		}
	} else {
		a_detruire = true;
	}

	colFromPic();
}

void EnnemiPrincesseguard::onMeure() {
	tombe();

	ss_etape += 1;
	ss_etape %= 6;

	if (ss_etape == 0 && etape < 10) etape += 1;

	if (etape >= 10) {
		int yy = plat(x, y);

		if (yy != 0 && yy != y_plat[4][x]) {
			grave(x, y, pic);
			a_detruire = true;
			game_flag[3] -= 1;
		}
	} else {
		pic = pbk_ennemis[138 + etape];
	}
}

void EnnemiPrincesseguard::estTouche(Tir* tir) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Ennemi::estTouche(tir);
	gicle(tir, dx_giclure, dy_giclure);
}
