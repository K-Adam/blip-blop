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

#define IDENNEMI_SMURF 0
#define IDENNEMI_SMURF_GOURMAND 1
#define IDENNEMI_SMURF_FARCEUR 2
#define IDENNEMI_SMURF_SAUVAGE 3
#define IDENNEMI_SMURF_PAYSAN 4
#define IDENNEMI_SMURCOSTAUD 5
#define IDENNEMI_SMURFETTE 6
#define IDENNEMI_CIGOGNE 7
#define IDENNEMI_GRANDSMURF 8

#define IDENNEMI_PIKACHU 100
#define IDENNEMI_BULBIZARRE 101
#define IDENNEMI_FLAMECHE 102
#define IDENNEMI_COM 103
#define IDENNEMI_COM_VOLANT 104

#define IDENNEMI_BISOUCIEL 200
#define IDENNEMI_BISOUETOILE 201
#define IDENNEMI_BISOUBOOM 202
#define IDENNEMI_BISOUNUAGE 203
#define IDENNEMI_BISOUZOMBI 204
#define IDENNEMI_BISOUCHAMAN 205
#define IDENNEMI_BISOUCOEUR 206
#define IDENNEMI_BISOUJEDI 207
#define IDENNEMI_BISOUJEDIHEROS 208
#define IDENNEMI_BISOUSIB 209

#define IDENNEMI_LEMMINGS 400
#define IDENNEMI_LEMMINGS_VOLANT 401

#define IDENNEMI_SNORKY_BASE_1 300
#define IDENNEMI_SNORKY_BASE_2 301
#define IDENNEMI_SNORKY_GOUVERNEUR 302
#define IDENNEMI_SNORKY_MAGE 303
#define IDENNEMI_SNORKY_RIDER 304
#define IDENNEMI_SNORK_IN_BLACK 305
#define IDENNEMI_DIABOLO 306
#define IDENNEMI_SNORK_IN_BLACK_BOSS 307

#define IDENNEMI_TOAD 500
#define IDENNEMI_YOSHI 501
#define IDENNEMI_PRINCESSE_GUARD 502
#define IDENNEMI_PRINCESSE 503
#define IDENNEMI_TAILS 504
#define IDENNEMI_YOSHI_DCA 505
#define IDENNEMI_KNUKLES 506
#define IDENNEMI_RAYMAN 507
#define IDENNEMI_TAILSVOLANT 508

#define IDENNEMI_LUIGI 600
#define IDENNEMI_SONIC 601
#define IDENNEMI_MARIO_TAPETTE 602
#define IDENNEMI_LARA 603
#define IDENNEMI_PIC 604
#define IDENNEMI_POID 605

#define IDENNEMI_MARIO 700

void EventEnemy::doEvent() {
	Enemy* enn = NULL;

	switch (id_ennemi) {
		case IDENNEMI_SMURF:
			switch (rand() % 6) {
				case 0:
					enn = new EnemySmurf();
					break;
				case 1:
					enn = new EnemySmurfGlasses();
					break;
				case 2:
					enn = new EnemySmurfVain();
					break;
				case 3:
					enn = new EnemySmurfChilly();
					break;
				case 4:
					enn = new EnemySmurfLazy();
					break;
				case 5:
					enn = new EnemySmurfPeon();
					break;
			}
			break;

		case IDENNEMI_SMURF_GOURMAND:
			enn = new EnemySmurfGreedy();
			break;

		case IDENNEMI_SMURF_FARCEUR:
			enn = new EnemySmurfJokey();
			break;

		case IDENNEMI_SMURF_SAUVAGE:
			enn = new EnemySmurfWild();
			break;

		case IDENNEMI_SMURF_PAYSAN:
			enn = new EnemySmurfFarmer();
			break;

		case IDENNEMI_SMURCOSTAUD:
			enn = new EnemySmurfHefty();
			break;

		case IDENNEMI_SMURFETTE:
			enn = new EnemySmurfette();
			break;

		case IDENNEMI_GRANDSMURF:
			enn = new EnemyPapaSmurf();
			break;

		case IDENNEMI_CIGOGNE:
			enn = new EnemySmurfStork();
			break;

		case IDENNEMI_PIKACHU:
			enn = new EnemyPikachu();
			break;

		case IDENNEMI_BULBIZARRE:
			enn = new EnemyBulbizarre();
			break;

		case IDENNEMI_FLAMECHE:
			enn = new EnemyEmber();
			break;

		case IDENNEMI_COM:
			enn = new EnemyCOM();
			break;

		case IDENNEMI_BISOUCIEL:
			enn = new EnemyBisouSky();
			break;

		case IDENNEMI_BISOUETOILE:
			enn = new EnemyBisouStar();
			break;

		case IDENNEMI_BISOUBOOM:
			enn = new EnemyBisouBoom();
			break;

		case IDENNEMI_BISOUNUAGE:
			enn = new EnemyBisouCloud();
			break;

		case IDENNEMI_BISOUZOMBI:
			enn = new EnemyBisouZombie();
			break;

		case IDENNEMI_BISOUCHAMAN:
			enn = new EnemyBisouShaman();
			break;

		case IDENNEMI_BISOUCOEUR:
			enn = new EnemyBisouHeart();
			break;

		case IDENNEMI_BISOUJEDI:
			enn = new EnemyBisouJedi();
			break;

		case IDENNEMI_BISOUJEDIHEROS:
			enn = new EnemyBisouJediHero();
			break;

		case IDENNEMI_BISOUSIB:
			enn = new EnemyBisouSIB();
			break;

		case IDENNEMI_LEMMINGS:
			enn = new EnemyLemmings();
			break;

		case IDENNEMI_LEMMINGS_VOLANT:
			enn = new EnemyLemmingsFlying();
			break;

		case IDENNEMI_COM_VOLANT:
			enn = new EnemyCOMFlying();
			break;

		case IDENNEMI_SNORKY_BASE_1:
			enn = new EnemySnorkyBase1();
			break;

		case IDENNEMI_SNORKY_BASE_2:
			enn = new EnemySnorkyBase2();
			break;

		case IDENNEMI_SNORKY_GOUVERNEUR:
			enn = new EnemySnorkyGovernor();
			break;

		case IDENNEMI_SNORKY_MAGE:
			enn = new EnemySnorkyMage();
			break;

		case IDENNEMI_SNORKY_RIDER:
			enn = new EnemySnorkyHyporider();
			break;

		case IDENNEMI_DIABOLO:
			enn = new EnemyDiabolo();
			break;

		case IDENNEMI_SNORK_IN_BLACK:
			enn = new EnemySnorkInBlack();
			break;

		case IDENNEMI_SNORK_IN_BLACK_BOSS:
			enn = new EnemySnorkInBlackBoss();
			break;

		case IDENNEMI_TOAD:
			enn = new EnemyToad();
			break;

		case IDENNEMI_YOSHI:
			enn = new EnemyYoshi();
			break;

		case IDENNEMI_YOSHI_DCA:
			enn = new EnemyYoshiAntiair();
			break;

		case IDENNEMI_PRINCESSE:
			enn = new EnemyPrincess();
			break;

		case IDENNEMI_TAILS:
			enn = new EnemyTails();
			break;

		case IDENNEMI_KNUKLES:
			enn = new EnemyKnuckles();
			break;

		case IDENNEMI_RAYMAN:
			enn = new EnemyRayman();
			break;

		case IDENNEMI_TAILSVOLANT:
			enn = new EnemyTailsFlying();
			break;

		case IDENNEMI_LUIGI:
			enn = new EnemyLuigi();
			break;

		case IDENNEMI_SONIC:
			enn = new EnemySonic();
			break;

		case IDENNEMI_MARIO_TAPETTE:
			enn = new EnemyMarioSwatter();
			break;

		case IDENNEMI_LARA:
			enn = new EnemyLara();
			break;

		case IDENNEMI_PIC:
			enn = new EnemySpikes();
			break;

		case IDENNEMI_POID:
			enn = new EnemyWeight();
			break;

		case IDENNEMI_MARIO:
			enn = new EnemyMario();
			break;
	}

	if (enn != NULL) {
		enn->x = x;
		enn->y = y;
		enn->dir = sens;

		list_ennemis.emplace_back(enn);

		if (enn->count()) nb_ennemis_created += 1;
	} else {
		debug << "Erreur : identité d'ennemi inconnue (" << id_ennemi << ")\n";
	}
}
