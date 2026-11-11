#pragma once

#include <list>
#include <memory>

#include "entities/bubble.h"
#include "level/weather_rain.h"
#include "level/weather_snow.h"

// Weather, underwater bubbles and screen distortion, the level dressing that no entity interacts with
class Effects {
   public:
	void update();
	void drawBubbles() const;
	void drawWeather() const;
	void drawDistortion() const;
	void removeDestroyed();
	void clear();

   private:
	static constexpr int NB_RAINDROPS = 500;
	static constexpr int NB_SNOWFLAKES = 500;

	void updateWeather();
	void updateBubbles();
	void spawnBubble(Sprite* s);

	// Drops are recycled from fixed pools, the list only points into them
	WeatherSnow snow_[NB_SNOWFLAKES];
	int next_snowflake_ = 0;
	WeatherRain rain_[NB_RAINDROPS];
	int next_raindrop_ = 0;
	std::list<Sprite*> weather_;

	std::list<std::unique_ptr<Bubble>> bubbles_;
	int phi_distort_ = 0;
};
