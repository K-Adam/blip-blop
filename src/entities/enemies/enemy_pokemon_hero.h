#pragma once

#include <string>

#include "entities/enemies/enemy.h"

struct PokemonHeroSpec {
	const char* name;  // Prefix of its release, walk and die animations
	int hp;
	int throw_dx;
	int throw_dy;
	int treasure;
	int land_sound;	 // -1 for none
	int speed;
	int die_frames;
	bool leaves_body;
};

// Thrown by Rayman inside a ball, it lands, comes out and then walks like a normal enemy
class EnemyPokemonHero : public Enemy {
   public:
	explicit EnemyPokemonHero(const PokemonHeroSpec& spec);
	void update() override;
	void onWalk();
	void onDie();
	void takeHit(Shot* shot) override;

   private:
	Flipbook anim(const std::string& action) const;
	Flipbook facing(const std::string& action) const;

	PokemonHeroSpec spec_;
	int dx;
	int dorkeball = 2;
};
