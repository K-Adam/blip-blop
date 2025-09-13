#pragma once

#include "entities/enemies/enemy.h"

#define SMURF_JOKEY_SPEED 2
#define STATE_DRAW_WEAPON 11

const int anim_smurf_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_smurf_walk_left[] = {5, 6, 7, 8, 9, 8, 7, 6};

class EnemySmurfJokey : public Enemy {
   public:
	bool carries_bomb;
	int new_bomb_delay;
	int wait_for_new_bomb;
	int bomb_delay;
	int wait_for_bomb;
	int gift_dy;

	EnemySmurfJokey();

	inline void render() override {
		if (carries_bomb) {
			if (dir == FACING_RIGHT)
				draw(x + 26, y - gift_dy, pbk_enemies[56]);
			else
				draw(x - 26, y - gift_dy, pbk_enemies[56]);
		}

		draw(x, y, pic);
	};

	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
	virtual void onDropGift();
	virtual void onDrawWeapon();
};
