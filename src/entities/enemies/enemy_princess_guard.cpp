#include "entities/enemies/enemy_princess_guard.h"
#include "entities/enemies/enemy_princess.h"

const int anim_guard_marche_gauche[] = {135, 136, 137, 136};

EnemyPrincessGuard::EnemyPrincessGuard(EnemyPrincess* Princesse, int distance) : princesse(Princesse), distance_couverture(distance) {
	hp = 2500;
	tresor = 100;
	dir = FACING_LEFT;
}

void EnemyPrincessGuard::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;
	}

	updateDestroyed();
}

void EnemyPrincessGuard::onAvance() {
	if (princesse->destroyed == true) {
		destroyed = true;
	}

	if (game_flag[2]) {
		status = ETAT_MEURE;
		onMeure();
		return;
	}
	fall();
	if (princesse != NULL) {
		if ((x + distance_couverture + GUARD_SPEED) < princesse->x) {
			marche(GUARD_SPEED);
			pic = pbk_enemies[animate(anim_guard_marche_gauche, 4, 6)];
		} else if ((x + distance_couverture - GUARD_SPEED) > princesse->x) {
			marche(-GUARD_SPEED * 3);
			pic = pbk_enemies[animate(anim_guard_marche_gauche, 4, 6)];
		} else {
			pic = pbk_enemies[135];
		}
	} else {
		destroyed = true;
	}

	colFromPic();
}

void EnemyPrincessGuard::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;

	if (anim_step >= 10) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) {
			engrave(x, y, pic);
			destroyed = true;
			game_flag[3] -= 1;
		}
	} else {
		pic = pbk_enemies[138 + anim_step];
	}
}

void EnemyPrincessGuard::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);
}
