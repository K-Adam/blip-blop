#include "entities/enemies/enemy_smurf_farmer.h"

const int smurf_paysan_charge_droite[] = {156, 157, 158, 159};
const int smurf_paysan_charge_gauche[] = {160, 161, 162, 163};

const int smurf_paysan_decapite_droite[] = {174, 175, 176, 177};
const int smurf_paysan_decapite_gauche[] = {178, 179, 180, 180};

EnemySmurfFarmer::EnemySmurfFarmer() {
	hp = 200;
	status = ETAT_AVANCE;
	tete_etape = 0;
	tete_ss_etape = 0;
}

void EnemySmurfFarmer::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case ETAT_MEURE:
			onMeure();
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;
		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemySmurfFarmer::onMeure() {
	if (tete_etape == 0) {
		x_mort = x;
		y_mort = y - 25;
	}

	if (tete_etape < 5) {
		tete_ss_etape += 1;
		tete_ss_etape %= 4;

		if (tete_ss_etape == 0) tete_etape += 1;
	}
}

void EnemySmurfFarmer::onAvance() {
	fall();

	if (solid_wall(x - SMURF_PAYSAN_SPEED, y))
		dir = FACING_RIGHT;
	else if (solid_wall(x + SMURF_PAYSAN_SPEED, y))
		dir = FACING_LEFT;

	if (status == ETAT_MEURE) {
		if (dir == FACING_RIGHT) {
			marche(SMURF_PAYSAN_SPEED);
			pic = pbk_enemies[animate(smurf_paysan_decapite_droite, 4, 3)];
		} else {
			marche(-SMURF_PAYSAN_SPEED);
			pic = pbk_enemies[animate(smurf_paysan_decapite_gauche, 4, 3)];
		}
	} else {
		if (dir == FACING_RIGHT) {
			marche(SMURF_PAYSAN_SPEED);
			pic = pbk_enemies[animate(smurf_paysan_charge_droite, 4, 3)];
		} else {
			marche(-SMURF_PAYSAN_SPEED);
			pic = pbk_enemies[animate(smurf_paysan_charge_gauche, 4, 3)];
		}
		colFromPic();
	}
}

void EnemySmurfFarmer::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}

void EnemySmurfFarmer::takeHit(Shot* shot) {
	static const int dx_giclure_smurf_paysan[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_smurf_paysan[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf_paysan, dy_giclure_smurf_paysan);

	if (status == ETAT_MEURE) sbk_misc.play(8 + rand() % 3);
}
