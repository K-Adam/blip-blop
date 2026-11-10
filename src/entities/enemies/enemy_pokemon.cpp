#include "entities/enemies/enemy_pokemon.h"

EnemyPokemon::EnemyPokemon(const PokemonSpec& spec) : spec_(spec) { hp = 100; }

Flipbook EnemyPokemon::facing(const std::string& action) const {
	return pbk_enemies.anim(spec_.name + ("_" + action) + (dir == FACING_RIGHT ? "_right" : "_left"));
}

void EnemyPokemon::update() {
	if (blood > 0) blood -= 1;

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

void EnemyPokemon::onWalk() {
	int speed = spec_.speed;

	fall();
	step(8, 2);

	if (x - speed < xmin || solid_wall(x - speed, y))
		dir = FACING_RIGHT;
	else if (x + speed > offset + 640 || solid_wall(x + speed, y))
		dir = FACING_LEFT;

	walk(dir == FACING_RIGHT ? speed : -speed);
	pic = facing("walk")[anim_step];

	colFromPic();
}

void EnemyPokemon::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= spec_.die_delay;

	if (anim_tick == 0 && anim_step < spec_.die_frames) anim_step += 1;

	if (anim_step >= spec_.die_frames) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		pic = facing("die")[anim_step];
	}
}

void EnemyPokemon::onCharred() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 14)
		destroyed = true;
	else
		pic = facing("burn")[anim_step];
}

void EnemyPokemon::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(8 + rand() % 2);
	}
}
