#include "level/events/event_enemy.h"

#include <unordered_map>
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

#include "entities/enemies/enemy_pokemon.h"
#include "entities/enemies/enemy_ember.h"
#include "entities/enemies/enemy_com.h"
#include "entities/enemies/enemy_com_flying.h"

#include "entities/enemies/enemy_lemmings.h"

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
#define ENEMYID_BULBASAUR 101
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

static const PokemonSpec PIKACHU{.name = "pikachu", .speed = 3, .die_delay = 4, .die_frames = 10};
static const PokemonSpec BULBASAUR{.name = "bulbasaur", .speed = 1, .die_delay = 5, .die_frames = 8};

template <class T>
static Enemy* make() {
	return new T();
}

static const std::unordered_map<int, Enemy* (*)()> ENEMIES = {
	{ENEMYID_SMURF,
	 []() -> Enemy* {
		 switch (rand() % 6) {
			 case 0:
				 return new EnemySmurf();
			 case 1:
				 return new EnemySmurfGlasses();
			 case 2:
				 return new EnemySmurfVain();
			 case 3:
				 return new EnemySmurfChilly();
			 case 4:
				 return new EnemySmurfLazy();
			 default:
				 return new EnemySmurfPeon();
		 }
	 }},
	{ENEMYID_SMURF_GREEDY, make<EnemySmurfGreedy>},
	{ENEMYID_SMURF_JOKEY, make<EnemySmurfJokey>},
	{ENEMYID_SMURF_WILD, make<EnemySmurfWild>},
	{ENEMYID_SMURF_FARMER, make<EnemySmurfFarmer>},
	{ENEMYID_SMURF_HEFTY, make<EnemySmurfHefty>},
	{ENEMYID_SMURFETTE, make<EnemySmurfette>},
	{ENEMYID_PAPA_SMURF, make<EnemyPapaSmurf>},
	{ENEMYID_SMURF_STORK, make<EnemySmurfStork>},
	{ENEMYID_PIKACHU, []() -> Enemy* { return new EnemyPokemon(PIKACHU); }},
	{ENEMYID_BULBASAUR, []() -> Enemy* { return new EnemyPokemon(BULBASAUR); }},
	{ENEMYID_EMBER, make<EnemyEmber>},
	{ENEMYID_COM, make<EnemyCOM>},
	{ENEMYID_BISOU_SKY, make<EnemyBisouSky>},
	{ENEMYID_BISOU_STAR, make<EnemyBisouStar>},
	{ENEMYID_BISOU_BOOM, make<EnemyBisouBoom>},
	{ENEMYID_BISOU_CLOUD, make<EnemyBisouCloud>},
	{ENEMYID_BISOU_ZOMBIE, make<EnemyBisouZombie>},
	{ENEMYID_BISOU_SHAMAN, make<EnemyBisouShaman>},
	{ENEMYID_BISOU_HEART, make<EnemyBisouHeart>},
	{ENEMYID_BISOU_JEDI, make<EnemyBisouJedi>},
	{ENEMYID_BISOU_JEDI_HERO, make<EnemyBisouJediHero>},
	{ENEMYID_BISOU_SIB, make<EnemyBisouSIB>},
	{ENEMYID_LEMMINGS, make<EnemyLemmings>},
	{ENEMYID_LEMMINGS_FLYING, make<EnemyLemmings>},
	{ENEMYID_COM_FLYING, make<EnemyCOMFlying>},
	{ENEMYID_SNORKY_BASE_1, make<EnemySnorkyBase1>},
	{ENEMYID_SNORKY_BASE_2, make<EnemySnorkyBase2>},
	{ENEMYID_SNORKY_GOVERNOR, make<EnemySnorkyGovernor>},
	{ENEMYID_SNORKY_MAGE, make<EnemySnorkyMage>},
	{ENEMYID_SNORKY_RIDER, make<EnemySnorkyHyporider>},
	{ENEMYID_DIABOLO, make<EnemyDiabolo>},
	{ENEMYID_SNORK_IN_BLACK, make<EnemySnorkInBlack>},
	{ENEMYID_SNORK_IN_BLACK_BOSS, make<EnemySnorkInBlackBoss>},
	{ENEMYID_TOAD, make<EnemyToad>},
	{ENEMYID_YOSHI, make<EnemyYoshi>},
	{ENEMYID_YOSHI_ANTIAIR, make<EnemyYoshiAntiair>},
	{ENEMYID_PRINCESS, make<EnemyPrincess>},
	{ENEMYID_TAILS, make<EnemyTails>},
	{ENEMYID_KNUCKLES, make<EnemyKnuckles>},
	{ENEMYID_RAYMAN, make<EnemyRayman>},
	{ENEMYID_TAILS_FLYING, []() -> Enemy* { return new EnemyTails(true); }},
	{ENEMYID_LUIGI, make<EnemyLuigi>},
	{ENEMYID_SONIC, make<EnemySonic>},
	{ENEMYID_MARIO_SWATTER, make<EnemyMarioSwatter>},
	{ENEMYID_LARA, make<EnemyLara>},
	{ENEMYID_SPIKES, make<EnemySpikes>},
	{ENEMYID_WEIGHT, make<EnemyWeight>},
	{ENEMYID_MARIO, make<EnemyMario>},
};

void EventEnemy::doEvent() {
	auto it = ENEMIES.find(enemy_id);
	if (it == ENEMIES.end()) {
		debug << "Unknown enemy id (" << enemy_id << ")\n";
		return;
	}

	Enemy* new_enemy = it->second();
	new_enemy->x = x;
	new_enemy->y = y;
	new_enemy->dir = facing;

	level().entities.enemies.emplace_back(new_enemy);

	if (new_enemy->count()) session().enemies_created += 1;
}
