#include "level/effects.h"

#include <cmath>
#include <numbers>

#include "engine/graphics.h"
#include "entities/ball.h"
#include "entities/enemies/enemy.h"
#include "level/world.h"

void Effects::update() {
	if (level().rules.flags[FLAG_BUBBLES]) updateBubbles();
	if (level().rules.weather_type == WEATHER_RAIN || level().rules.weather_type == WEATHER_SNOW) updateWeather();
	if (level().rules.weather_type == WEATHER_DISTORT && level().rules.weather_intensity != 0) phi_distort_ = (phi_distort_ + 2) % 360;
}

void Effects::drawBubbles() const {
	if (!level().rules.flags[FLAG_BUBBLES]) return;
	for (auto& b : bubbles_) b->render();
}

void Effects::drawWeather() const {
	for (Sprite* drop : weather_) drop->render();
}

void Effects::removeDestroyed() {
	std::erase_if(bubbles_, [](auto& b) { return b->isDestroyed(); });
	std::erase_if(weather_, [](Sprite* drop) { return drop->isDestroyed(); });
}

void Effects::clear() {
	weather_.clear();
	bubbles_.clear();
}

void Effects::updateWeather() {
	for (Sprite* drop : weather_) drop->update();

	while (weather_.size() < static_cast<size_t>(level().rules.weather_intensity)) {
		if (level().rules.weather_type == WEATHER_SNOW) {
			next_snowflake_ = (next_snowflake_ + 1) % NB_SNOWFLAKES;
			WeatherSnow* snowflake = &snow_[next_snowflake_];

			int d = rnd() % 3;

			snowflake->destroyed = false;
			snowflake->xbase = level().camera.offset + rnd() % 800;
			snowflake->y = rnd() % 480 - 500;

			if (d <= 1)
				snowflake->pic = assets().pics.misc.pic("snowflake_small");
			else
				snowflake->pic = assets().pics.misc.pic("snowflake_large");

			snowflake->dy = level().rules.weather_intensity / 20 + d;
			snowflake->phi = rnd() % 360;
			snowflake->xwide = 10 + d * 4;

			if (level().terrain.solid_wall(snowflake->xbase, 0)) snowflake->y -= 550;

			weather_.emplace_back(snowflake);
		} else if (level().rules.weather_type == WEATHER_RAIN) {
			next_raindrop_ = (next_raindrop_ + 1) % NB_RAINDROPS;
			WeatherRain* raindrop = &rain_[next_raindrop_];

			int d = rnd() % 4;

			raindrop->destroyed = false;
			raindrop->x = level().camera.offset + rnd() % 800;
			raindrop->y = rnd() % 480 - 480;
			raindrop->pic = assets().pics.misc.anim("raindrop")[d];
			raindrop->dy = 7 + d * 2;

			if (level().terrain.solid_wall(raindrop->x, 0)) raindrop->y -= 550;

			weather_.emplace_back(raindrop);
		}
	}
}

void Effects::updateBubbles() {
	for (Ball* pl : session().players) {
		spawnBubble(pl);
	}

	for (auto& pl : level().entities.enemies) {
		if (bubbles_.size() < 15) {
			break;
		}
		spawnBubble(pl.get());
	}

	for (auto& b : bubbles_) {
		b->update();
	}
}

void Effects::spawnBubble(Sprite* s) {
	s->wait_bubble += rnd() % 10;

	if (s->wait_bubble > 300) {
		auto b = std::make_unique<Bubble>();

		b->xbase = s->x;
		b->y = s->y - 10;
		b->pic = assets().pics.misc.anim("bubble")[rnd() % 3];
		b->dphi = 5 + rnd() % 3;
		b->dy = -(2 + rnd() % 2);

		bubbles_.emplace_back(std::move(b));

		s->wait_bubble = 0;
	}
}

void Effects::drawDistortion() const {
	if (level().rules.weather_type != WEATHER_DISTORT || level().rules.weather_intensity == 0) return;

	const int stride = 2;
	int phi = phi_distort_;
	Rect r;

	for (int y = 0; y < 480; y += stride) {
		phi = (phi + 1) % 360;

		r.top = y;
		r.bottom = y + stride;

		int x = static_cast<int>(5 * std::sin(phi * std::numbers::pi / 180));
		int xt = x;

		if (x < 0) {
			r.left = -x;
			r.right = 640;
			x = 0;
		} else {
			r.left = 0;
			r.right = 640 - x;
		}

		graphics.back()->BltFast(x, y, graphics.back(), &r);

		if (xt < 0) {
			r.left = 640 + xt;
			r.right = 640;
		} else {
			r.left = 0;
			r.right = xt;
		}

		graphics.back()->FillRect(&r, 0);
	}
}
