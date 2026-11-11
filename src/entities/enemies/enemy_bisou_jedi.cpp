#include "entities/enemies/enemy_bisou_jedi.h"
#include "entities/shots/shot_laser_sword.h"

EnemyBisouJedi::EnemyBisouJedi() : shoot_step(0), has_sword(true) {
	hp = 250;
	wait_shoot = 200 + rnd() % 250;
}

void EnemyBisouJedi::update() {
	if (blood > 0) blood -= 1;

	if (has_sword) shoot_step += 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemyBisouJedi::onWalk() {
	static const int SPEED = 1;

	fall();

	step(6, 8);

	if (x - SPEED < level().camera.offset + 20 || level().terrain.solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > level().camera.offset + 600 || level().terrain.solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = assets().pics.enemies.anim("jedi_walk_right")[anim_step];
	} else {
		walk(-SPEED);
		pic = assets().pics.enemies.anim("jedi_walk_left")[anim_step];
	}

	if (shoot_step > wait_shoot && session().targeted_player != nullptr) {
		auto t = std::make_unique<ShotLaserSword>();

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->thrower = this;
		t->target = session().targeted_player;

		level().entities.enemy_shots.emplace_back(std::move(t));

		shoot_step = 0;
		wait_shoot = 200 + rnd() % 250;
		has_sword = false;
	}

	colFromPic();
}

void EnemyBisouJedi::onDie() {
	if (has_sword) {
		auto t = std::make_unique<ShotLaserSword>();

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->thrower = this;

		if (session().targeted_player == nullptr)
			t->isdead = true;
		else
			t->target = session().targeted_player;

		level().entities.enemy_shots.emplace_back(std::move(t));

		shoot_step = 0;
		wait_shoot = 200 + rnd() % 250;
		has_sword = false;
	}

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 20) {
			if (level().terrain.platform_y(x, y) != 0) engrave(x, y, pic);

			destroyed = true;
			return;
		}
	}

	pic = facing(assets().pics.enemies, "jedi_die")[anim_step];
}

void EnemyBisouJedi::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}

void EnemyBisouJedi::onCharred() {
	if (has_sword) {
		auto t = std::make_unique<ShotLaserSword>();

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->thrower = this;

		if (session().targeted_player == nullptr)
			t->isdead = true;
		else
			t->target = session().targeted_player;

		level().entities.enemy_shots.emplace_back(std::move(t));

		shoot_step = 0;
		wait_shoot = 200 + rnd() % 250;
		has_sword = false;
	}

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 7) {
			destroyed = true;
			return;
		}
	}

	pic = facing(assets().pics.enemies, "jedi_charred")[anim_step];
}

void EnemyBisouJedi::render() {
	static const int dx_sword[] = {8, 4, -1, -6, -6, -6, -1, 3};
	static const int dy_sword[] = {-19, -19, -19, -19, -18, -18, -19, -19};
	static const int sword_frames[] = {0, 1, 2, 3, 4, 3, 2, 1};

	Sprite::render();

	if (has_sword) {
		if (dir == FACING_RIGHT)
			draw(x + dx_sword[anim_step], y + dy_sword[anim_step], assets().pics.enemies.anim("jedi_sword_right")[sword_frames[anim_step]]);
		else
			draw(x - dx_sword[anim_step], y + dy_sword[anim_step], assets().pics.enemies.anim("jedi_sword_left")[sword_frames[anim_step]]);
	}
}
