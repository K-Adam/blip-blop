#pragma once

#include "entities/enemies/enemy.h"

#define DIAGONAL_LEFT 0
#define DIAGONAL_DOWN_LEFT 1
#define DOWN_LEFT 2
#define DOWN_RIGHT 3
#define DIAGONAL_DOWN_RIGHT 4
#define DIAGONAL_RIGHT 5

#define STATE_WAITING 100
#define STATE_CLIMB 102

class EnemySmurfWild : public Enemy {
   public:
	int shoot_direction;
	int height_px;
	int slope;	// Slope of the line through tete_turc and this smurf
	int num_pic;
	int shoot_step;
	int nb_shoot;

	static int wait_shoot;
	static int wait_step;

	bool first_loop;
	bool dead;
	bool position;

	int angle_tick;
	int angle;
	bool angleIncreasing;

	bool shooting;

	Sprite* target;

	EnemySmurfWild();

	inline void render() override {
		draw(x, y - 50, pbk_enemies[117]);
		draw(x, y - 100, pbk_enemies[117]);
		draw(x, y - 150, pbk_enemies[117]);
		draw(x, y - 200, pbk_enemies[117]);
		draw(x, y - 250, pbk_enemies[117]);

		Enemy::render();

		if (hp > 0) {
			if (shooting) {
				switch (shoot_direction) {
					case DIAGONAL_LEFT:
						draw(x + 8, y + 38, pbk_enemies[137 + anim_step]);
						break;
					case DIAGONAL_DOWN_LEFT:
						draw(x + 8, y + 38, pbk_enemies[133 + anim_step]);
						break;
					case DOWN_LEFT:
						draw(x + 8, y + 38, pbk_enemies[129 + anim_step]);
						break;
					case DOWN_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[127 + anim_step]);
						break;
					case DIAGONAL_DOWN_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[131 + anim_step]);
						break;
					case DIAGONAL_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[135 + anim_step]);
						break;
				}
			} else {
				switch (shoot_direction) {
					case DIAGONAL_LEFT:
						draw(x + 8, y + 38, pbk_enemies[352]);
						break;
					case DIAGONAL_DOWN_LEFT:
						draw(x + 8, y + 38, pbk_enemies[350]);
						break;
					case DOWN_LEFT:
						draw(x + 8, y + 38, pbk_enemies[354]);
						break;
					case DOWN_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[353]);
						break;
					case DIAGONAL_DOWN_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[349]);
						break;
					case DIAGONAL_RIGHT:
						draw(x - 5, y + 38, pbk_enemies[351]);
						break;
				}
			}
		}
	}

	void update() override;
	virtual void onFall();
	virtual void onShoot();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
