#include "entities/enemies/enemy_smurf_farmer.h"

const int smurf_farmer_charge_right[] = {156, 157, 158, 159};
const int smurf_farmer_charge_left[] = {160, 161, 162, 163};

const int smurf_farmer_beheaded_right[] = {174, 175, 176, 177};
const int smurf_farmer_beheaded_left[] = {178, 179, 180, 180};

EnemySmurfFarmer::EnemySmurfFarmer() {
	hp = 200;
	status = STATE_WALK;
	head_step = 0;
	head_tick = 0;
}

void EnemySmurfFarmer::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_DYING:
			onDie();
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;
		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemySmurfFarmer::onDie() {
	if (head_step == 0) {
		x_death = x;
		y_death = y - 25;
	}

	if (head_step < 5) {
		head_tick += 1;
		head_tick %= 4;

		if (head_tick == 0) head_step += 1;
	}
}

void EnemySmurfFarmer::onWalk() {
	fall();

	if (solid_wall(x - SMURF_FARMER_SPEED, y))
		dir = FACING_RIGHT;
	else if (solid_wall(x + SMURF_FARMER_SPEED, y))
		dir = FACING_LEFT;

	if (status == STATE_DYING) {
		if (dir == FACING_RIGHT) {
			walk(SMURF_FARMER_SPEED);
			pic = pbk_enemies[animate(smurf_farmer_beheaded_right, 4, 3)];
		} else {
			walk(-SMURF_FARMER_SPEED);
			pic = pbk_enemies[animate(smurf_farmer_beheaded_left, 4, 3)];
		}
	} else {
		if (dir == FACING_RIGHT) {
			walk(SMURF_FARMER_SPEED);
			pic = pbk_enemies[animate(smurf_farmer_charge_right, 4, 3)];
		} else {
			walk(-SMURF_FARMER_SPEED);
			pic = pbk_enemies[animate(smurf_farmer_charge_left, 4, 3)];
		}
		colFromPic();
	}
}

void EnemySmurfFarmer::onCharred() {
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
	static const int dx_splatter_smurf_farmer[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf_farmer[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf_farmer, dy_splatter_smurf_farmer);

	if (status == STATE_DYING) sbk_misc.play(8 + rand() % 3);
}
