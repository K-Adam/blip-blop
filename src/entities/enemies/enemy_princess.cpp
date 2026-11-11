#include "entities/enemies/enemy_princess.h"
#include "entities/enemies/enemy_princess_guard.h"
#include "entities/cool_text.h"
#include "entities/bonus_item.h"

EnemyPrincess::EnemyPrincess() : walking(false), guards_called(false) {
	hp = 3700;
	treasure = 0;
	pic = assets().pics.enemies.pic("princess_idle");
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

	if (level().rules.flags[1]) {
		updateDestroyed();
	}
}

void EnemyPrincess::onWalk() {
	if (level().rules.flags[1]) {
		if (x > level().camera.offset + 820) {
			auto txt = std::make_unique<CoolText>();
			txt->ntxt = 517;
			level().entities.cool_texts.emplace_back(std::move(txt));

			destroyed = true;
		}
		if (!guards_called) {
			guards_called = true;
			std::unique_ptr<Enemy> guard;
			int i;
			for (i = 1; i < 4; i++) {
				guard = std::make_unique<EnemyPrincessGuard>(this, 40 + i * 40);

				guard->x = level().camera.offset + 720 - i * 20;
				guard->y = y - 20;
				guard->dir = FACING_LEFT;
				level().entities.enemies.emplace_back(std::move(guard));
			}
			session().enemies_created += 3;
		}
		if (dir == FACING_RIGHT) {
			if (x - level().camera.offset > 400) {
				step(6, 2);
				walk(PRINCESS_SPEED);
				pic = assets().pics.enemies.anim("princess_walk")[anim_step];
			} else {
				step(6, 2);
				walk(2);
				pic = assets().pics.enemies.anim("princess_walk")[anim_step];
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
			pic = assets().pics.enemies.anim("princess_walk")[anim_step];
		} else if (x > 7500) {
			pic = assets().pics.enemies.anim("princess_walk")[0];
		} else {
			pic = assets().pics.enemies.pic("princess_idle");
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
		int yy = level().terrain.platform_y(x, y);

		if (yy != 0 && yy != level().terrain.platforms[4][x]) {
			engrave(x, y, pic);
			auto txt = std::make_unique<CoolText>();
			txt->ntxt = 519;
			level().entities.cool_texts.emplace_back(std::move(txt));
			destroyed = true;
			if (level().rules.flags[3] > 0) {
				level().rules.flags[2] = 1;
			}

			auto bonus = std::make_unique<BonusCow>();
			bonus->x = x;
			bonus->y = y;
			level().entities.bonus.emplace_back(std::move(bonus));
		}
	} else {
		pic = assets().pics.enemies.anim("princess_die")[anim_step];
	}
}

void EnemyPrincess::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (level().rules.flags[1]) {
		Enemy::takeHit(shot);
	}
	splatter(shot, dx_splatter, dy_splatter);
}
