#include "entities/enemies/enemy_lemmings.h"
#include "entities/bonus_cow.h"

static int wait_no = 0;

EnemyLemmings::EnemyLemmings() : suicide(false) { hp = 100; }

void EnemyLemmings::update() {
	if (blood != 0) blood -= 1;

	// After 200 kills the lemmings suicide and the generators stop
	if (game_flag[FLAG_NB_KILL] >= 200 && status != ETAT_MEURE && status != ETAT_CARBONISE) {
		suicide = true;
		col_on = false;
		anim_tick = 0;
		anim_step = 0;
		status = ETAT_MEURE;
		game_flag[FLAG_GEN_OFF] = 1;
	}

	if (game_flag[3] == 1) {
		sbk_level.play(5);
		game_flag[3] = 0;
	}

	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onTombe();
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

void EnemyLemmings::onAvance() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 14;
	}

	if (dir == FACING_LEFT) {
		x -= LEM_SPEED;

		fall();

		pic = pbk_enemies[14 + anim_step];

		if (solid_wall(x, y) || x < 5) dir = FACING_RIGHT;
	} else {
		x += LEM_SPEED;

		fall();

		pic = pbk_enemies[anim_step];

		if (solid_wall(x, y)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemyLemmings::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemyLemmings::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 6 && (platform_y(x - 80, y) == 0 || platform_y(x + 80, y) == 0)) destroyed = true;

	if (anim_step == 10) {
		if (platform_y(x, y) != 0 && platform_y(x - 80, y) != 0 && platform_y(x + 80, y) != 0) engrave(x, y, pic);

		destroyed = true;

		if (suicide && game_flag[0] == 0) {
			Bonus* bonus = new BonusCow();
			bonus->x = 1500;
			bonus->y = -50;

			list_bonus.emplace_back(bonus);
			game_flag[0] = 1;
		}
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[38 + anim_step];
		else
			pic = pbk_enemies[28 + anim_step];
	}
}

void EnemyLemmings::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 14) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[62 + anim_step];
		else
			pic = pbk_enemies[48 + anim_step];
	}
}

void EnemyLemmings::takeHit(Shot* shot) {
	static const int dx_giclure_lem[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_lem[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_lem, dy_giclure_lem);

	if (status == ETAT_MEURE) {
		wait_no += 1;
		wait_no %= 20;

		if (wait_no == 0) {
			sbk_level.play(1);
		} else {
			sbk_misc.play(7 + rand() % 3);
		}
	}
}
