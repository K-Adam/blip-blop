#pragma once

#include "entities/enemies/enemy.h"

#define DIAGONALE_GAUCHE 0
#define DIAGONALE_BAS_GAUCHE 1
#define BAS_GAUCHE 2
#define BAS_DROITE 3
#define DIAGONALE_BAS_DROITE 4
#define DIAGONALE_DROITE 5

#define ETAT_ATTENDS 100
#define ETAT_MONTE 102

class EnemySmurfWild : public Enemy {
   public:
	int shoot_direction;
	int hauteur;
	int pente;	// Slope of the line through tete_turc and this smurf
	int num_pic;
	int etape_shoot;
	int nb_shoot;

	static int wait_shoot;
	static int etape_wait;

	bool first_loop;
	bool mort;
	bool position;

	int ss_etape_angle;
	int angle;
	bool sensAngle;

	bool shooting;

	Sprite* cible;

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
					case DIAGONALE_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[137 + anim_step]);
						break;
					case DIAGONALE_BAS_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[133 + anim_step]);
						break;
					case BAS_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[129 + anim_step]);
						break;
					case BAS_DROITE:
						draw(x - 5, y + 38, pbk_enemies[127 + anim_step]);
						break;
					case DIAGONALE_BAS_DROITE:
						draw(x - 5, y + 38, pbk_enemies[131 + anim_step]);
						break;
					case DIAGONALE_DROITE:
						draw(x - 5, y + 38, pbk_enemies[135 + anim_step]);
						break;
				}
			} else {
				switch (shoot_direction) {
					case DIAGONALE_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[352]);
						break;
					case DIAGONALE_BAS_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[350]);
						break;
					case BAS_GAUCHE:
						draw(x + 8, y + 38, pbk_enemies[354]);
						break;
					case BAS_DROITE:
						draw(x - 5, y + 38, pbk_enemies[353]);
						break;
					case DIAGONALE_BAS_DROITE:
						draw(x - 5, y + 38, pbk_enemies[349]);
						break;
					case DIAGONALE_DROITE:
						draw(x - 5, y + 38, pbk_enemies[351]);
						break;
				}
			}
		}
	}

	void update() override;
	virtual void onTombe();
	virtual void onTire();
	virtual void onMeure();
	virtual void onCarbonise();
	void takeHit(Shot* shot) override;
};
