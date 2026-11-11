#pragma once

#include <string>

#include "entities/enemies/enemy.h"

struct PokemonSpec {
	const char* name;  // Prefix of its walk, die and burn animations
	int speed;
	int die_delay;
	int die_frames;
};

class EnemyPokemon : public Enemy {
   public:
	explicit EnemyPokemon(const PokemonSpec& spec);
	void update() override;
	void onWalk();
	void onDie();
	void onCharred();
	void takeHit(Shot* shot) override;

   private:
	Flipbook facing(const std::string& action) const;

	PokemonSpec spec_;
};
