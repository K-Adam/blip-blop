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
	const char* hang = "smurf_wild_hang_right";
	int shoot_step;
	int nb_shoot;

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
		draw(x, y - 50, assets().pics.enemies.pic("smurf_wild_rope"));
		draw(x, y - 100, assets().pics.enemies.pic("smurf_wild_rope"));
		draw(x, y - 150, assets().pics.enemies.pic("smurf_wild_rope"));
		draw(x, y - 200, assets().pics.enemies.pic("smurf_wild_rope"));
		draw(x, y - 250, assets().pics.enemies.pic("smurf_wild_rope"));

		Enemy::render();

		if (hp > 0) {
			if (shooting) {
				switch (shoot_direction) {
					case DIAGONAL_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[9 + anim_step]);
						break;
					case DIAGONAL_DOWN_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[5 + anim_step]);
						break;
					case DOWN_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[1 + anim_step]);
						break;
					case DOWN_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[anim_step - 1]);
						break;
					case DIAGONAL_DOWN_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[3 + anim_step]);
						break;
					case DIAGONAL_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_fire")[7 + anim_step]);
						break;
				}
			} else {
				switch (shoot_direction) {
					case DIAGONAL_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[3]);
						break;
					case DIAGONAL_DOWN_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[1]);
						break;
					case DOWN_LEFT:
						draw(x + 8, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[5]);
						break;
					case DOWN_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[4]);
						break;
					case DIAGONAL_DOWN_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[0]);
						break;
					case DIAGONAL_RIGHT:
						draw(x - 5, y + 38, assets().pics.enemies.anim("smurf_wild_gun_idle")[2]);
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
