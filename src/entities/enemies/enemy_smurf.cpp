#include "entities/enemies/enemy_smurf.h"

const int anim_smurf_marche_droite[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_smurf_marche_gauche[] = {5, 6, 7, 8, 9, 8, 7, 6};

EnemySmurf::EnemySmurf() : wait_for_jump(0) {
	hp = 100;
	jump_delay = 50 + rand() % 250;
	dy_saut = -5;
	speed = 1;
}

void EnemySmurf::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onSaute();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemySmurf::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onSaute();
	}

	wait_for_jump += 1;

	if (wait_for_jump > jump_delay) {
		status = ETAT_SAUTE;
		dy = dy_saut;
		lat_grav = 0;  // Otherwise jump heights vary
		wait_for_jump = 0;
		jump_delay = 50 + rand() % 250;
		onSaute();
	}

	if (x - speed < xmin || solid_wall(x - speed, y))
		dir = FACING_RIGHT;
	else if (x + speed > offset + 640 || solid_wall(x + speed, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[animate(anim_smurf_marche_droite, 8, 5)];
	} else {
		marche(-speed);
		pic = pbk_enemies[animate(anim_smurf_marche_gauche, 8, 5)];
	}

	colFromPic();
}

void EnemySmurf::onSaute() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies[10];
	} else {
		x -= speed;
		pic = pbk_enemies[11];
	}

	colFromPic();
}

void EnemySmurf::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 10) {
		destroyed = true;
		return;
	}

	if (dir == FACING_LEFT) {
		if (!solid_wall(x - speed, y) && platform_y(x, y) == 0) x -= speed;

		pic = pbk_enemies[23 + anim_step];
	} else {
		if (!solid_wall(x + speed, y) && platform_y(x, y) == 0) x += speed;

		pic = pbk_enemies[12 + anim_step];
	}
}

void EnemySmurf::takeHit(Shot* shot) {
	static const int dx_giclure_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf, dy_giclure_smurf);

	if (status == ETAT_MEURE) sbk_level.play(4 + rand() % 10);
}

void EnemySmurf::onCarbonise() {
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
