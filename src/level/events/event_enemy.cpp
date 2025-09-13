#include "level/events/event_enemy.h"
#include "entities/enemies/enemy.h"

#include "entities/enemies/enemy_smurf.h"
#include "entities/enemies/enemy_smurf_greedy.h"
#include "entities/enemies/enemy_smurf_jokey.h"
#include "entities/enemies/enemy_smurf_wild.h"
#include "entities/enemies/enemy_smurf_glasses.h"
#include "entities/enemies/enemy_smurf_lazy.h"
#include "entities/enemies/enemy_smurf_vain.h"
#include "entities/enemies/enemy_smurf_chilly.h"
#include "entities/enemies/enemy_smurf_peon.h"
#include "entities/enemies/enemy_smurf_farmer.h"
#include "entities/enemies/enemy_smurf_hefty.h"
#include "entities/enemies/enemy_smurfette.h"
#include "entities/enemies/enemy_papa_smurf.h"
#include "entities/enemies/enemy_smurf_stork.h"

#include "entities/enemies/enemy_pikachu.h"
#include "entities/enemies/enemy_bulbizarre.h"
#include "entities/enemies/enemy_ember.h"
#include "entities/enemies/enemy_com.h"
#include "entities/enemies/enemy_com_flying.h"

#include "entities/enemies/enemy_lemmings.h"
#include "entities/enemies/enemy_lemmings_flying.h"

#include "entities/enemies/enemy_bisou_sky.h"
#include "entities/enemies/enemy_bisou_star.h"
#include "entities/enemies/enemy_bisou_boom.h"
#include "entities/enemies/enemy_bisou_cloud.h"
#include "entities/enemies/enemy_bisou_zombie.h"
#include "entities/enemies/enemy_bisou_shaman.h"
#include "entities/enemies/enemy_bisou_heart.h"
#include "entities/enemies/enemy_bisou_jedi.h"
#include "entities/enemies/enemy_bisou_sib.h"

#include "entities/enemies/enemy_snorky_base_1.h"
#include "entities/enemies/enemy_snorky_base_2.h"
#include "entities/enemies/enemy_snorky_governor.h"
#include "entities/enemies/enemy_snorky_mage.h"
#include "entities/enemies/enemy_snorky_hyporider.h"
#include "entities/enemies/enemy_snork_in_black.h"
#include "entities/enemies/enemy_snork_in_black_boss.h"
#include "entities/enemies/enemy_diabolo.h"

#include "entities/enemies/enemy_toad.h"
#include "entities/enemies/enemy_yoshi.h"
#include "entities/enemies/enemy_yoshi_antiair.h"
#include "entities/enemies/enemy_princess_guard.h"
#include "entities/enemies/enemy_princess.h"
#include "entities/enemies/enemy_tails.h"
#include "entities/enemies/enemy_tails_flying.h"
#include "entities/enemies/enemy_knuckles.h"
#include "entities/enemies/enemy_rayman.h"

#include "entities/enemies/enemy_luigi.h"
#include "entities/enemies/enemy_sonic.h"
#include "entities/enemies/enemy_mario_swatter.h"
#include "entities/enemies/enemy_lara.h"
#include "entities/enemies/enemy_spikes.h"
#include "entities/enemies/enemy_weight.h"

#include "entities/enemies/enemy_mario.h"

#define ENEMYID_SMURF 0
#define ENEMYID_SMURF_GREEDY 1
#define ENEMYID_SMURF_JOKEY 2
#define ENEMYID_SMURF_WILD 3
#define ENEMYID_SMURF_FARMER 4
#define ENEMYID_SMURF_HEFTY 5
#define ENEMYID_SMURFETTE 6
#define ENEMYID_SMURF_STORK 7
#define ENEMYID_PAPA_SMURF 8

#define ENEMYID_PIKACHU 100
#define ENEMYID_BULBIZARRE 101
#define ENEMYID_EMBER 102
#define ENEMYID_COM 103
#define ENEMYID_COM_FLYING 104

#define ENEMYID_BISOU_SKY 200
#define ENEMYID_BISOU_STAR 201
#define ENEMYID_BISOU_BOOM 202
#define ENEMYID_BISOU_CLOUD 203
#define ENEMYID_BISOU_ZOMBIE 204
#define ENEMYID_BISOU_SHAMAN 205
#define ENEMYID_BISOU_HEART 206
#define ENEMYID_BISOU_JEDI 207
#define ENEMYID_BISOU_JEDI_HERO 208
#define ENEMYID_BISOU_SIB 209

#define ENEMYID_LEMMINGS 400
#define ENEMYID_LEMMINGS_FLYING 401

#define ENEMYID_SNORKY_BASE_1 300
#define ENEMYID_SNORKY_BASE_2 301
#define ENEMYID_SNORKY_GOVERNOR 302
#define ENEMYID_SNORKY_MAGE 303
#define ENEMYID_SNORKY_RIDER 304
#define ENEMYID_SNORK_IN_BLACK 305
#define ENEMYID_DIABOLO 306
#define ENEMYID_SNORK_IN_BLACK_BOSS 307

#define ENEMYID_TOAD 500
#define ENEMYID_YOSHI 501
#define ENEMYID_PRINCESS_GUARD 502
#define ENEMYID_PRINCESS 503
#define ENEMYID_TAILS 504
#define ENEMYID_YOSHI_ANTIAIR 505
#define ENEMYID_KNUCKLES 506
#define ENEMYID_RAYMAN 507
#define ENEMYID_TAILS_FLYING 508

#define ENEMYID_LUIGI 600
#define ENEMYID_SONIC 601
#define ENEMYID_MARIO_SWATTER 602
#define ENEMYID_LARA 603
#define ENEMYID_SPIKES 604
#define ENEMYID_WEIGHT 605

#define ENEMYID_MARIO 700

void EventEnemy::doEvent() {
	Enemy* new_enemy = NULL;

	switch (enemy_id) {
		case ENEMYID_SMURF:
			switch (rand() % 6) {
				case 0:
					new_enemy = new EnemySmurf();
					break;
				case 1:
					new_enemy = new EnemySmurfGlasses();
					break;
				case 2:
					new_enemy = new EnemySmurfVain();
					break;
				case 3:
					new_enemy = new EnemySmurfChilly();
					break;
				case 4:
					new_enemy = new EnemySmurfLazy();
					break;
				case 5:
					new_enemy = new EnemySmurfPeon();
					break;
			}
			break;

		case ENEMYID_SMURF_GREEDY:
			new_enemy = new EnemySmurfGreedy();
			break;

		case ENEMYID_SMURF_JOKEY:
			new_enemy = new EnemySmurfJokey();
			break;

		case ENEMYID_SMURF_WILD:
			new_enemy = new EnemySmurfWild();
			break;

		case ENEMYID_SMURF_FARMER:
			new_enemy = new EnemySmurfFarmer();
			break;

		case ENEMYID_SMURF_HEFTY:
			new_enemy = new EnemySmurfHefty();
			break;

		case ENEMYID_SMURFETTE:
			new_enemy = new EnemySmurfette();
			break;

		case ENEMYID_PAPA_SMURF:
			new_enemy = new EnemyPapaSmurf();
			break;

		case ENEMYID_SMURF_STORK:
			new_enemy = new EnemySmurfStork();
			break;

		case ENEMYID_PIKACHU:
			new_enemy = new EnemyPikachu();
			break;

		case ENEMYID_BULBIZARRE:
			new_enemy = new EnemyBulbizarre();
			break;

		case ENEMYID_EMBER:
			new_enemy = new EnemyEmber();
			break;

		case ENEMYID_COM:
			new_enemy = new EnemyCOM();
			break;

		case ENEMYID_BISOU_SKY:
			new_enemy = new EnemyBisouSky();
			break;

		case ENEMYID_BISOU_STAR:
			new_enemy = new EnemyBisouStar();
			break;

		case ENEMYID_BISOU_BOOM:
			new_enemy = new EnemyBisouBoom();
			break;

		case ENEMYID_BISOU_CLOUD:
			new_enemy = new EnemyBisouCloud();
			break;

		case ENEMYID_BISOU_ZOMBIE:
			new_enemy = new EnemyBisouZombie();
			break;

		case ENEMYID_BISOU_SHAMAN:
			new_enemy = new EnemyBisouShaman();
			break;

		case ENEMYID_BISOU_HEART:
			new_enemy = new EnemyBisouHeart();
			break;

		case ENEMYID_BISOU_JEDI:
			new_enemy = new EnemyBisouJedi();
			break;

		case ENEMYID_BISOU_JEDI_HERO:
			new_enemy = new EnemyBisouJediHero();
			break;

		case ENEMYID_BISOU_SIB:
			new_enemy = new EnemyBisouSIB();
			break;

		case ENEMYID_LEMMINGS:
			new_enemy = new EnemyLemmings();
			break;

		case ENEMYID_LEMMINGS_FLYING:
			new_enemy = new EnemyLemmingsFlying();
			break;

		case ENEMYID_COM_FLYING:
			new_enemy = new EnemyCOMFlying();
			break;

		case ENEMYID_SNORKY_BASE_1:
			new_enemy = new EnemySnorkyBase1();
			break;

		case ENEMYID_SNORKY_BASE_2:
			new_enemy = new EnemySnorkyBase2();
			break;

		case ENEMYID_SNORKY_GOVERNOR:
			new_enemy = new EnemySnorkyGovernor();
			break;

		case ENEMYID_SNORKY_MAGE:
			new_enemy = new EnemySnorkyMage();
			break;

		case ENEMYID_SNORKY_RIDER:
			new_enemy = new EnemySnorkyHyporider();
			break;

		case ENEMYID_DIABOLO:
			new_enemy = new EnemyDiabolo();
			break;

		case ENEMYID_SNORK_IN_BLACK:
			new_enemy = new EnemySnorkInBlack();
			break;

		case ENEMYID_SNORK_IN_BLACK_BOSS:
			new_enemy = new EnemySnorkInBlackBoss();
			break;

		case ENEMYID_TOAD:
			new_enemy = new EnemyToad();
			break;

		case ENEMYID_YOSHI:
			new_enemy = new EnemyYoshi();
			break;

		case ENEMYID_YOSHI_ANTIAIR:
			new_enemy = new EnemyYoshiAntiair();
			break;

		case ENEMYID_PRINCESS:
			new_enemy = new EnemyPrincess();
			break;

		case ENEMYID_TAILS:
			new_enemy = new EnemyTails();
			break;

		case ENEMYID_KNUCKLES:
			new_enemy = new EnemyKnuckles();
			break;

		case ENEMYID_RAYMAN:
			new_enemy = new EnemyRayman();
			break;

		case ENEMYID_TAILS_FLYING:
			new_enemy = new EnemyTailsFlying();
			break;

		case ENEMYID_LUIGI:
			new_enemy = new EnemyLuigi();
			break;

		case ENEMYID_SONIC:
			new_enemy = new EnemySonic();
			break;

		case ENEMYID_MARIO_SWATTER:
			new_enemy = new EnemyMarioSwatter();
			break;

		case ENEMYID_LARA:
			new_enemy = new EnemyLara();
			break;

		case ENEMYID_SPIKES:
			new_enemy = new EnemySpikes();
			break;

		case ENEMYID_WEIGHT:
			new_enemy = new EnemyWeight();
			break;

		case ENEMYID_MARIO:
			new_enemy = new EnemyMario();
			break;
	}

	if (new_enemy != NULL) {
		new_enemy->x = x;
		new_enemy->y = y;
		new_enemy->dir = facing;

		list_enemies.emplace_back(new_enemy);

		if (new_enemy->count()) nb_enemies_created += 1;
	} else {
		debug << "Erreur : identité d'ennemi inconnue (" << enemy_id << ")\n";
	}
}
