#pragma once

#include "entities/sprite.h"

class BackgroundSnorkAirlock1 : public Sprite {
   private:
	int players_inside;

   public:
	BackgroundSnorkAirlock1();
	void update() override;
	void render() override;
};

class BackgroundSnorkAirlock2 : public Sprite {
   public:
	BackgroundSnorkAirlock2();
	void update() override;
	void render() override;
};
