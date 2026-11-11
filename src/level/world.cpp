#include <vector>
#include <list>
#include <memory>

#include "level/world.h"
#include "level/backgrounds/background_static.h"
#include "entities/shots/shot_bb_cow.h"
#include "entities/vehicle.h"
#include "entities/enemies/enemy.h"
#include "level/enemy_generator.h"
#include "entities/bonus.h"
#include "level/bonus_generator.h"
#include "entities/sprite.h"
#include "entities/cool_text.h"
#include "entities/splatter.h"
#include "entities/ball.h"

Level::Level() = default;
Level::~Level() = default;

World::World() : level_(std::make_unique<Level>()) { current_ = this; }
World::~World() { current_ = nullptr; }

void World::resetLevel() {
	level_ = std::make_unique<Level>();
	scroll.reset();
}

bool Camera::Lock::released(const Entities& entities, const int* flags) const {
	switch (cond) {
		case NoEnemies:
			return entities.enemies.empty();
		case NoGenerators:
			return entities.enemy_gens.empty();
		case FlagEquals:
			return flags[flag] == val;
		case FlagAtLeast:
			return flags[flag] >= val;
	}
	return false;
}

void Camera::clamp(int level_width) {
	if (offset < 0)
		offset = 0;
	else if (offset > level_width - 640)
		offset = level_width - 640;
}

void Camera::follow(const std::vector<Ball*>& players, bool forced, int level_width) {
	if (lock.on) {
		if (offset < lock.x) offset = lock.x;
	} else if (speed != 0 && forced) {
		offset += speed;
	} else if (players.size() > 0) {
		int x_avg = 0;

		for (Ball* s : players) {
			x_avg += s->x;
		}

		x_avg /= static_cast<int>(players.size());
		x_avg -= 320;  // Half of the 640 screen width

		if (x_avg > offset) {
			if ((x_avg - offset) >= 2)
				offset += 2;
			else
				offset = x_avg;
		}
	}

	clamp(level_width);
}

// Moving platforms can reach past the level edges
void Terrain::set_platform(int n, int x, int y) {
	if (x >= 0 && static_cast<size_t>(x) < width) platforms[n][x] = y;
}

int Terrain::platform_y(int x, int y) const {
	if (x < 0 || x >= static_cast<int>(width) || y >= 480) return 0;

	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		int tmp = platforms[i][x];

		if (y >= tmp && y <= tmp + 10) return tmp;
	}
	return 0;
}

int Terrain::platform_y2(int x, int y) const {
	if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= 480) return -1;

	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		int tmp = platforms[i][x];

		if (y >= tmp && y <= tmp + 10) return i;
	}

	return -1;
}

bool Terrain::solid_wall(int x, int y) const {
	if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= 480) return false;

	return solid[y / 8][x / 8];
}

bool Terrain::bloody_wall(int x, int y) const {
	if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= 480) return false;

	return bloody[y / 8][x / 8];
}

bool engrave(int x, int y, const Picture* pic) {
	if (pic == nullptr || x > (level().camera.offset + ScrollRenderer::WIDTH - pic->xSize())) return false;

	BackgroundStatic s;

	s.x = x;
	s.y = y;
	s.pic = pic;

	s.colFromPic();

	for (auto& s2 : level().entities.animated_bgs) {
		if (s.collision(s2.get())) return false;
	}

	for (auto& s2 : level().entities.moving_platforms) {
		if (s.collision(s2.get())) return false;
	}

	return World::current().scroll.stamp(x, y, pic);
}
