#include "entities/enemies/enemy_princess.h"
#include "entities/enemies/enemy_princess_guard.h"
#include "entities/cool_text.h"
#include "entities/bonus_cow.h"

EnemyPrincess::EnemyPrincess() : walking(false), guards_called(false) {
	hp = 3700;
	treasure = 0;
	pic = pbk_enemies.pic("princess_idle");
}

void EnemyPrincess::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onDie();
			break;
	}

	if (game_flag[1]) {
		updateDestroyed();
	}
}

void EnemyPrincess::onWalk() {
	if (game_flag[1]) {
		if (x > offset + 820) {
			CoolText* txt = new CoolText();
			txt->ntxt = 517;
			list_txt_cool.emplace_back(txt);

			destroyed = true;
		}
		if (!guards_called) {
			guards_called = true;
			Enemy* guard;
			int i;
			for (i = 1; i < 4; i++) {
				guard = new EnemyPrincessGuard(this, 40 + i * 40);

				guard->x = offset + 720 - i * 20;
				guard->y = y - 20;
				guard->dir = FACING_LEFT;
				list_enemies.emplace_back(guard);
			}
			nb_enemies_created += 3;
		}
		if (dir == FACING_RIGHT) {
			if (x - offset > 400) {
				step(6, 2);
				walk(PRINCESS_SPEED);
				pic = pbk_enemies.anim("princess_walk")[anim_step];
			} else {
				step(6, 2);
				walk(2);
				pic = pbk_enemies.anim("princess_walk")[anim_step];
			}
		} else if (dir == FACING_LEFT) {
			dir = FACING_RIGHT;
		} else {
			walking = false;
		}
	} else {
		if ((x < 5760)) {
			step(6, 2);
			walk(PRINCESS_SPEED);
			pic = pbk_enemies.anim("princess_walk")[anim_step];
		} else if (x > 7500) {
			pic = pbk_enemies.anim("princess_walk")[0];
		} else {
			pic = pbk_enemies.pic("princess_idle");
		}
	}
	colFromPic();
}

void EnemyPrincess::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 6) anim_step += 1;

	if (anim_step >= 5) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) {
			engrave(x, y, pic);
			CoolText* txt = new CoolText();
			txt->ntxt = 519;
			list_txt_cool.emplace_back(txt);
			destroyed = true;
			if (game_flag[3] > 0) {
				game_flag[2] = 1;
			}

			Bonus* bonus = new BonusCow();
			bonus->x = x;
			bonus->y = y;
			list_bonus.emplace_back(bonus);
		}
	} else {
		pic = pbk_enemies.anim("princess_die")[anim_step];
	}
}

void EnemyPrincess::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (game_flag[1]) {
		Enemy::takeHit(shot);
	}
	splatter(shot, dx_splatter, dy_splatter);
}
