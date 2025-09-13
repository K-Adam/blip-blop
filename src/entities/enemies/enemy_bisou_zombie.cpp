#include "entities/enemies/enemy_bisou_zombie.h"
#include "entities/gore_feet_bisou_zombie.h"

#define ETAT_CARBONISE_HAUT 102

#define ETAT_RAMPE 100
#define ETAT_COUPE 101

static int etape_moan = 199;
static int num_moan = 0;
static bool local_phase;

EnemyBisouZombie::EnemyBisouZombie() : etape2(0) {
	hp = 1000;
	tresor = 10;
}

void EnemyBisouZombie::update() {
	if (blood > 0) blood -= 1;

	if (game_flag[0] == 7 && status != ETAT_CARBONISE && status != ETAT_CARBONISE_HAUT) {
		anim_step = anim_tick = 0;

		if (status == ETAT_COUPE)
			status = ETAT_CARBONISE_HAUT;
		else
			status = ETAT_CARBONISE;
	}

	if (local_phase != phase && game_flag[2] == 0) {
		etape_moan += 1;
		etape_moan %= 200;

		if (etape_moan == 0) {
			num_moan += 1;
			num_moan %= 3;

			sbk_level.play(10 + num_moan);
		}

		local_phase = phase;
	}

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_COUPE:
			onCoupe();
			break;

		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_RAMPE:
			onRampe();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_CARBONISE_HAUT:
			onCarboniseHaut();
			break;
	}

	updateDestroyed();
}

void EnemyBisouZombie::onAvance() {
	static const int SPEED = 1;

	fall();

	anim_tick += 1;
	anim_tick %= 15;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	my_phase = !my_phase;

	if (my_phase) {
		if (x - SPEED < xmin || solid_wall(x - SPEED, y))
			dir = FACING_RIGHT;
		else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
			dir = FACING_LEFT;

		if (dir == FACING_RIGHT) {
			marche(SPEED);
			pic = pbk_enemies[262 + anim_step];
		} else {
			marche(-SPEED);
			pic = pbk_enemies[270 + anim_step];
		}
	}

	colFromPic();
}

void EnemyBisouZombie::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	static const int dy_giclure2[] = {0, -10, -10, -10, -20, -10, -10, -10};

	int back_etat = status;
	int back_etape = anim_step;
	int back_ss_etape = anim_tick;

	int blessure = shot->damage();

	if (blessure < hp) {
		shot->aTouche(blessure);
		hp -= blessure;

		if (hp < 500 && (status == STATE_NORMAL || status == ETAT_AVANCE) && !shot->enflame()) {
			anim_step = anim_tick = 0;
			col_on = false;
			status = ETAT_COUPE;
		}
	} else {
		shot->aTouche(hp);
		shot->aTue();
		hp = 0;
		col_on = false;

		game_flag[FLAG_NB_KILL] += 1;

		if (status == ETAT_RAMPE) {
			anim_step = 0;
			anim_tick = 0;

			if (shot->enflame() == 2) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = ETAT_CARBONISE_HAUT;
			} else {
				anim_step = anim_tick = 0;
				col_on = false;
				status = ETAT_MEURE;
			}
		} else {
			if (shot->enflame()) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = ETAT_CARBONISE;
			} else if (status != ETAT_COUPE) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = ETAT_COUPE;
			}
		}
	}

	if (status == ETAT_RAMPE)
		gicle(shot, dx_giclure, dy_giclure2);
	else
		gicle(shot, dx_giclure, dy_giclure);
}

void EnemyBisouZombie::onTombe() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[308 + anim_step];
		x += 1;
	} else {
		pic = pbk_enemies[312 + anim_step];
		x -= 1;
	}

	fall();

	if (platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = ETAT_RAMPE;
		col_on = true;
		onRampe();
	}
}

void EnemyBisouZombie::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[292 + anim_step];
	else
		pic = pbk_enemies[296 + anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouZombie::onCoupe() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		GoreFeetBisouZombie* pieds = new GoreFeetBisouZombie();

		pieds->x = x;
		pieds->dir = dir;
		pieds->y = y;

		list_gore.emplace_back(pieds);

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_TOMBE;
		onTombe();
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[278 + anim_step];
		else
			pic = pbk_enemies[282 + anim_step];
	}
}

void EnemyBisouZombie::onRampe() {
	static const int anim_droite[] = {286, 287, 288, 287};
	static const int anim_gauche[] = {289, 290, 291, 290};
	static const int SPEED = 1;

	if (hp <= 0) {
		anim_step = anim_tick = 0;
		status = ETAT_MEURE;
		col_on = false;
		onMeure();
		return;
	}

	fall();

	etape2 = (etape2 + 1) % 4;

	if (etape2 == 0) {
		if (x - SPEED < xmin || solid_wall(x - SPEED, y))
			dir = FACING_RIGHT;
		else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
			dir = FACING_LEFT;

		if (dir == FACING_RIGHT)
			marche(SPEED);
		else
			marche(-SPEED);

		colFromPic();
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[animate(anim_droite, 4, 16)];
	else
		pic = pbk_enemies[animate(anim_gauche, 4, 16)];
}

void EnemyBisouZombie::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 6) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[605 + anim_step];
	else
		pic = pbk_enemies[612 + anim_step];
}

void EnemyBisouZombie::onCarboniseHaut() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 7) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[619 + anim_step];
	else
		pic = pbk_enemies[627 + anim_step];
}
