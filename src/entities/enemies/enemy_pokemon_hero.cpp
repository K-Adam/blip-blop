#include "entities/enemies/enemy_pokemon_hero.h"

EnemyPokemonHero::EnemyPokemonHero(const PokemonHeroSpec& spec) : spec_(spec), dx(spec.throw_dx) {
	hp = spec.hp;
	dy = spec.throw_dy;
	treasure = spec.treasure;
}

Flipbook EnemyPokemonHero::anim(const std::string& action) const { return assets().pics.enemies.anim(spec_.name + ("_" + action)); }

Flipbook EnemyPokemonHero::facing(const std::string& action) const { return anim(action + (dir == FACING_RIGHT ? "_right" : "_left")); }

void EnemyPokemonHero::update() {
	if (dorkeball == 2) {
		x += dx;
		fall();
		int yp;
		if ((dy > 0) && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
			if (spec_.land_sound >= 0) assets().sounds.level.play(spec_.land_sound);
			status = STATE_NORMAL;
			dy = 0;
			y = yp;
			anim_step = 0;
			anim_tick = 0;
			dorkeball = 1;
			return;
		} else {
			pic = assets().pics.enemies.anim("dorkeball_spin")[step(5, 4)];
		}
	} else if (dorkeball == 1) {
		anim_tick++;
		anim_tick %= 8;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step > 3) {
				dorkeball = 0;
				anim_step = 0;
				anim_tick = 0;
			} else {
				pic = anim("release")[anim_step];
			}
		}

	} else {
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

		updateDestroyed();
	}
}

void EnemyPokemonHero::onWalk() {
	fall();

	step(8, 2);

	if (x - 1 < xmin || level().terrain.solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > level().camera.offset + 640 || level().terrain.solid_wall(x + 1, y))
		dir = FACING_LEFT;

	walk(dir == FACING_RIGHT ? spec_.speed : -spec_.speed);
	pic = facing("walk")[anim_step];
	colFromPic();
}

void EnemyPokemonHero::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0 && anim_step < spec_.die_frames) anim_step += 1;

	if (anim_step >= spec_.die_frames) {
		int yy = level().terrain.platform_y(x, y);

		if (spec_.leaves_body && yy != 0 && yy != level().terrain.platforms[4][x]) engrave(x, y, pic);

		destroyed = true;
		level().rules.flags[1]--;
	} else {
		pic = facing("die")[anim_step];
	}
}

void EnemyPokemonHero::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (dorkeball == 0) {
		Enemy::takeHit(shot);
		splatter(shot, dx_splatter, dy_splatter);
	}
}
