#include "entities/enemies/enemy_yoshi.h"
#include "entities/shots/shot_shell.h"

const int anim_yoshi_marche_droite[] = {28, 29, 30, 29};
const int anim_yoshi_marche_gauche[] = {31, 32, 33, 32};
const int anim_yoshi_attaque_droite[] = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34};
const int anim_yoshi_attaque_gauche[] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44};

EnemyYoshi::EnemyYoshi() : attack_delay(50 + rand() % 250), wait_for_attack(0) {
	tresor = 10;
	pv = 500;
}

void EnemyYoshi::update() {
	if (blood > 0) blood -= 1;

	switch (etat) {
		case ETAT_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_TIRE:	 // Yoshi tire la langue ou balance des carapaces
			if (attack) {
				onTire();
			} else {
				onAttaque();
			}
			break;
	}

	updateADetruire();
}

void EnemyYoshi::onAvance() {
	// Si plus de plateformes on passe dans l'etat TOMBE
	if (plat(x, y) == 0) {
		etat = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		etape = 0;
		ss_etape = 0;
		onTombe();
		return;
	}

	// pour attaquer
	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		etape = 0;
		etat = ETAT_TIRE;
		attack = true;
		etape = 0;
		ss_etape = 0;
		sbk_niveau.play(35);
		onTire();
		return;
	}

	if ((plat2(tete_turc->x, tete_turc->y) == plat2(x, y)) && (((dir == SENS_DROITE) && (x > (tete_turc->x - 150)) && (tete_turc->x > x)) ||
															   ((dir == SENS_GAUCHE) && (x < (tete_turc->x + 150)) && (tete_turc->x < x)))) {
		etape = 0;
		etat = ETAT_TIRE;
		attack = false;
		etape = 0;
		ss_etape = 0;
		sbk_niveau.play(37);
		onAttaque();
		return;
	}

	// pour marcher

	if (x - YOSHI_SPEED < xmin || mur_opaque(x - YOSHI_SPEED, y)) {
		dir = SENS_DROITE;
	} else if (x + YOSHI_SPEED > offset + 640 || mur_opaque(x + YOSHI_SPEED, y)) {
		dir = SENS_GAUCHE;
	}

	if (dir == SENS_DROITE) {
		marche(YOSHI_SPEED);
		pic = pbk_ennemis[anime(anim_yoshi_marche_droite, 3, 3)];
	} else {
		marche(-YOSHI_SPEED);
		pic = pbk_ennemis[anime(anim_yoshi_marche_gauche, 3, 3)];
	}
	colFromPic();
}

void EnemyYoshi::onTombe() {
	int yp;

	tombe();

	// Et si on arrêtait de tomber ?
	if (dy > 0 && (yp = plat(x, y + dy)) != 0) {
		etat = ETAT_NORMAL;
		dy = 0;
		y = yp;
		etape = 0;
		ss_etape = 0;
		onAvance();
		return;
	}

	if (dir == SENS_DROITE) {
		x++;
		pic = pbk_ennemis[28];
	} else {
		x--;
		pic = pbk_ennemis[31];
	}

	colFromPic();
}

void EnemyYoshi::onMeure() {
	tombe();

	ss_etape += 1;
	ss_etape %= 6;

	if (ss_etape == 0 && etape < 9) etape += 1;

	if (etape >= 9) {
		int yy = plat(x, y);

		if (yy != 0 && yy != y_plat[4][x]) grave(x, y, pic);

		a_detruire = true;
	} else {
		if (dir == SENS_GAUCHE)
			pic = pbk_ennemis[71 + etape];
		else
			pic = pbk_ennemis[62 + etape];
	}
}

void EnemyYoshi::onAttaque() {
	if (dir == SENS_DROITE) {
		pic = pbk_ennemis[anime(anim_yoshi_attaque_droite, 18, 2)];
	} else {
		pic = pbk_ennemis[anime(anim_yoshi_attaque_gauche, 18, 1)];
	}
	if (etape >= 17) {
		etat = ETAT_AVANCE;
		etape = 0;
		ss_etape = 0;
		onAvance();
		return;
	}
	colFromPic();
}

void EnemyYoshi::onTire() {
	ss_etape += 1;
	ss_etape %= 15;

	if (ss_etape == 0) etape += 1;

	if (etape >= 2) {
		ShotShell* tir = new ShotShell();

		tir->setDir(dir);

		if (dir == SENS_GAUCHE)
			tir->x = x - 23;
		else
			tir->x = x + 23;

		tir->y = y - 23;

		list_tirs_ennemis.emplace_back(tir);

		etat = ETAT_AVANCE;
		etape = 0;
		ss_etape = 0;
		onAvance();
		return;
	}

	if (dir == SENS_DROITE) {
		pic = pbk_ennemis[54 + etape];
	} else {
		pic = pbk_ennemis[56 + etape];
	}

	colFromPic();
}

void EnemyYoshi::onCarbonise() {
	tombe();
	ss_etape++;
	ss_etape %= 5;

	if (ss_etape == 0) etape++;

	if (etape >= 14)
		a_detruire = true;
	else {
		if (dir == SENS_DROITE)
			pic = pbk_ennemis[460 + etape];
		else
			pic = pbk_ennemis[474 + etape];
	}
}

void EnemyYoshi::estTouche(Shot* tir) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::estTouche(tir);
	gicle(tir, dx_giclure, dy_giclure);

	if (etat == ETAT_MEURE) {
		sbk_misc.play(6 + rand() % 3);
	}
}
