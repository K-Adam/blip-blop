#include <cmath>
#include <numbers>
#include <algorithm>

#include <fcntl.h>
#include <cstring>
#include <fstream>

#include "core/debug.h"
#include "entities/blip.h"
#include "entities/blop.h"
#include "entities/bonus.h"
#include "entities/bubble.h"
#include "menus/character_selection.h"
#include "menus/join_screen.h"
#include "cinematics/cine_player.h"
#include "core/app.h"
#include "engine/graphics.h"
#include "entities/enemies/enemy.h"
#include "level/events/event_bonus.h"
#include "level/events/event_enemy.h"
#include "level/events/event_animated_background.h"
#include "level/events/event_bonus_generator.h"
#include "level/events/event_enemy_generator.h"
#include "level/events/event_hold_fire.h"
#include "level/events/event_lock.h"
#include "level/events/event_weather.h"
#include "level/events/event_midground.h"
#include "level/events/event_music.h"
#include "level/events/event_foreground.h"
#include "level/events/event_rpg.h"
#include "level/events/event_scroll_speed.h"
#include "level/events/event_set_flag.h"
#include "level/events/event_sound.h"
#include "level/events/event_text.h"
#include "level/events/event_vehicle.h"
#include "entities/explosion.h"
#include "level/events/event_types.h"
#include "core/game.h"
#include "level/bonus_generator.h"
#include "level/enemy_generator.h"
#include "level/world.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "entities/make_bonus.h"
#include "menus/menu_game.h"
#include "menus/title_screen.h"
#include "level/weather_snow.h"
#include "level/weather_rain.h"
#include "entities/cool_text.h"
#include "entities/shots/shot_bb.h"
#include "entities/shots/shot_bb_cow.h"
#include "entities/vehicle.h"
#include <json.h>
#include "core/asset_path.h"
#include <base64.h>
#include "engine/half_tone.h"

using json = nlohmann::json;

#define TXT_TIME 828

// Out of line so the owned entity types are complete here
Game::Game() = default;
Game::~Game() = default;

Task<> Game::playGame(int player_count, int player_idx) {
	if (in.quitRequested()) co_return;

	CINEPlayer cine;
	bool letsgo = true;
	int i;

	assets().music.interl.stop();

	if (!loadGame()) {
		releaseGame();
		co_return;
	}

	wait_for_death = 0;

	if (player_idx == 0)
		player1 = std::make_unique<Blip>();
	else
		player1 = std::make_unique<Blop>();

	player1->ctrl = &controllers_[0];
	session().players.push_back(player1.get());

	if (player_count == 2) {
		if (player_idx == 0)
			player2 = std::make_unique<Blop>();
		else
			player2 = std::make_unique<Blip>();

		player2->ctrl = &controllers_[1];
		session().players.push_back(player2.get());
	}

	session().cow_bomb = false;
	last_perfect1 = last_perfect2 = false;
	i = 0;

	while (i < nb_part && letsgo && !in.quitRequested()) {
		if (type_part[i] == PART_LEVEL) {
			letsgo = co_await playLevel(file_names[i].c_str(), type_lvl[i]);

			if (!session().cow_bomb) {
				if (player1 != NULL) player1->nb_cow_bomb = 1;

				if (player2 != NULL) player2->nb_cow_bomb = 1;

				session().cow_bomb = true;
			}
		} else if (type_part[i] == PART_BRIEFING) {
			showBriefing(file_names[i].c_str());
		} else {
			co_await cine.playScene(file_names[i].c_str(), graphics.back());
		}

		i += 1;
		co_await nextFrame();
	}

	if (letsgo && !in.quitRequested()) {
		MusicBank mbk;
		MusicBank mbk2;
		PictureBank pbk;

		pbk.loadGFX("theend.gfx");
		mbk.open("credits.mbk");
		mbk2.open("end.mbk", false);
		cine.loadPBK("end.gfx");
		mbk.play(1);
		co_await cine.playScene("end.cin", graphics.back());
		mbk.stop();

		pbk.pic("cow_crush")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();
		pbk.pic("cow_crush")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();

		if (!in.anyKeyPressed()) mbk2.play(0);

		co_await in.waitClean();
		co_await in.waitKey();
		co_await in.waitClean();
		mbk2.stop();

		mbk.play(0);
		co_await showCredits(true);

		pbk.pic("the_end")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();
		pbk.pic("the_end")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();

		co_await in.waitClean();
		co_await in.waitKey();
		co_await in.waitClean();

		mbk.stop();

		co_await showPE(false);
	}

	releaseGame();

	assets().pics.inter.loadGFX("inter.gfx");

	if (!skipped && !in.quitRequested()) {
		if (!letsgo) {
			co_await showGameOver();
		}

		co_await getHiscore();
		co_await showHighScores();
	}

	player1.reset();
	player2.reset();
}

Task<bool> Game::playLevel(const char* level_name, int type) {
	// Restored after a bonus level
	int p1_life = 0;
	bool p1_bringBack = false;
	int p2_life = 0;
	bool p2_bringBack = false;

	players_dead = false;
	level_done = false;
	skipped = false;
	wait_for_victory = -1;

	timer_tick = 0;
	go_.Reset();

	world_.resetLevel();

	if (!loadLevel(level_name)) co_return false;

	level().rules.flamethrower_ok = std::string_view(level_name) != "lvl/snorkniv.lvl" && std::string_view(level_name) != "lvl/snorkniv2.lvl";

	if (player1 != NULL && player1->nb_life > 0) {
		player1->x = xstart1;
		player1->y = ystart1;
		player1->status = STATE_NORMAL;
		player1->anim_step = 0;
		player1->anim_tick = 0;
		player1->dir = BBDIR_RIGHT;
		player1->is_firing = false;

		if (type == LVL_BONUS) {
			p1_life = player1->nb_life;
			p1_bringBack = true;

			if (p1_life > 0) player1->nb_life = 1;
		}
	}

	if (player2 != NULL && player2->nb_life > 0) {
		player2->x = xstart2;
		player2->y = ystart2;
		player2->status = STATE_NORMAL;
		player2->anim_step = 0;
		player2->anim_tick = 0;
		player2->dir = BBDIR_RIGHT;
		player2->is_firing = false;

		if (type == LVL_BONUS) {
			p2_life = player2->nb_life;
			p2_bringBack = true;

			if (p2_life > 0) player2->nb_life = 1;
		}
	}

	if (type == LVL_BONUS || type == LVL_COMPLETE || type == LVL_FIRST || type == LVL_END) {
		session().enemies_created = 0;

		if (player1 != NULL) {
			player1->setKilled(0);

			if (type != LVL_END) {
				player1->status = STATE_COME_BACK;
				player1->y = -50;
				player1->y_to_go = ystart1;
			}

			player1->perfect = true;
			player1->hp = 5;

			if (last_perfect1) {
				player1->setSuperWeapon();
			}
		}

		if (player2 != NULL) {
			player2->setKilled(0);

			if (type != LVL_END) {
				player2->status = STATE_COME_BACK;
				player2->y = -50;
				player2->y_to_go = ystart2;
			}

			player2->perfect = true;
			player2->hp = 5;

			if (last_perfect2) {
				player2->setSuperWeapon();
			}
		}
	}

	last_perfect1 = last_perfect2 = false;

	bonus_level = type == LVL_BONUS;

	Sprite* s = session().players[0];
	level().camera.offset = s->x;
	level().camera.offset -= (level().camera.offset % 640);

	for (Chrono pause; pause.elapsed() < 2000 && !in.quitRequested();) {
		graphics.Flip();
		co_await nextFrame();
	}

	if (briefing) {
		Rect r;

		r.left = 0;
		r.right = 640;
		r.top = 0;
		r.bottom = 480;

		graphics.back()->FillRect(&r, 0);

		pbk_briefing.pic("briefing")->PasteTo(graphics.back(), 0, 0);
		assets().fonts.rpg.printC(graphics.back(), 320, 460, "Press a key to start.");
		graphics.Flip();

		assets().music.inter.play(2);
		co_await in.waitClean();
		co_await in.waitKey();
		briefing = false;
	}

	if (!in.quitRequested()) debug << "Now getting into game loop.\n";

	updateEvents();
	updateAll();

	update_regulator_.Skip();

	while (!players_dead && !level_done && !skipped && !in.quitRequested()) {
		co_await gameLoop();
		updateVictoryAndDefeat();

		co_await nextFrame();
	}

	if (players_dead) debug << "Players died\n";
	if (level_done) debug << "Level complete\n";
	if (skipped) debug << "User skipped\n";
	if (in.quitRequested()) debug << "Application killed\n";

	if (player1 != NULL) {
		player1->endLevel();
	}

	if (player2 != NULL) {
		player2->endLevel();
	}

	assets().music.level.stop();

	// A bonus level gives the lives back and cannot be lost
	if (type == LVL_BONUS && !skipped && !in.quitRequested()) {
		if (player1 != NULL && p1_bringBack) {
			if (player1->destroyed && p1_life > 0) {
				player1->destroyed = false;
				session().players.push_back(player1.get());
			}

			player1->nb_life = p1_life;
		}

		if (player2 != NULL && p2_bringBack) {
			if (player2->destroyed && p2_life > 0) {
				player2->destroyed = false;
				session().players.push_back(player2.get());
			}

			player2->nb_life = p2_life;
		}

		level_done = true;
	}

	if (level_done && !players_dead && type != LVL_END) {
		drawAll(false);
		graphics.Flip();
		drawAll(false);

		if ((type == LVL_BONUS && !skipped && level().rules.flags[FLAG_TIMER] > 0) || type == LVL_COMPLETE || type == LVL_LAST) {
			if (player1 != NULL) {
				last_perfect1 = player1->perfect;
			}

			if (player2 != NULL) {
				last_perfect2 = player2->perfect;
			}

			co_await showPE((type == LVL_BONUS));
		} else if (type == LVL_BONUS) {
			co_await showPE(true, true);
		}
	} else if (type == LVL_BONUS) {
		co_await showPE(true, true);
	}

	drawAll(false);
	drawLoading();
	graphics.Flip();

	releaseLevel();

	co_return level_done;
}

bool Game::loadLevel(const char* level_name) {
	auto dirname = asset_path(level_name);
	auto json_path = dirname + ".json";

	std::ifstream input(json_path);
	if (!input.good()) {
		debug << "Game::chargeNiveau() -> Cannot load <" << json_path << ">\n";
		return false;
	}

	json data = json::parse(input);

	debug << "---------------------------------------------------------------\n";
	debug << "Loading level <" << level_name << ">\n";
	debug << "---------------------------------------------------------------\n";

	// Every asset but the background is optional, an empty name means the level has none
	auto load = [&](const char* key, const char* dir, const char* what, bool required, auto loader) {
		std::string name = data[key].get<std::string>();
		if (name.empty() && !required) return true;

		std::string path = dir + name;
		if (!loader(path.c_str())) {
			debug << "Game::chargeNiveau() -> Cannot load " << path << " as " << what << "\n";
			return false;
		}
		debug << "Successfully loaded <" << path << "> as " << what << "\n";
		return true;
	};

	bool loaded = load("gfx_decors", "gfx/", "background", true, [](const char* f) { return assets().pics.decor.loadGFX(f); }) &&
				  load("gfx_niveau", "gfx/", "level stuff", false, [](const char* f) { return assets().pics.level.loadGFX(f); }) &&
				  load("gfx_ennemis", "gfx/", "ennemies", false, [](const char* f) { return assets().pics.enemies.loadGFX(f); }) &&
				  load("sbk_ennemis", "sfx/", "SBK", false, [](const char* f) { return assets().sounds.level.loadSFX(f); }) &&
				  load("fichier_mbk", "mbk/", "MBK", false, [](const char* f) { return assets().music.level.open(f); }) &&
				  load("fichier_rpg", "rpg/", "RPG file", false,
					   [&](const char* f) {
						   rpg.attachFile(f);
						   return true;
					   }) &&
				  load("gfx_rpg", "gfx/", "RPG GFX", false, [](const char* f) { return assets().pics.rpg.loadGFX(f); });
	if (!loaded) return false;

	Terrain& terrain = level().terrain;
	terrain.screens = data["num_decor"].size();
	terrain.width = terrain.screens * 640;

	auto platform_str = from_base64(data["y_plat"]);
	auto solid_walls_str = from_base64(data["murs_opaques"]);
	auto bloody_walls_str = from_base64(data["murs_sanglants"]);
	size_t level_size_8 = terrain.width / 8;
	if (platform_str.size() < NB_MAX_PLATFORMS * terrain.width * sizeof(int32_t) || solid_walls_str.size() < 60 * level_size_8 ||
		bloody_walls_str.size() < 60 * level_size_8) {
		debug << "Game::chargeNiveau() -> Truncated collision data in <" << json_path << ">\n";
		return false;
	}

	terrain.decor.resize(terrain.screens);
	for (size_t i = 0; i < terrain.screens; i++) {
		terrain.decor[i] = data["num_decor"][i];
	}

	xstart1 = data["xstart1"];
	ystart1 = data["ystart1"];
	xstart2 = data["xstart2"];
	ystart2 = data["ystart2"];

	vic_x = data["vic_x"];
	vic_flag1 = data["vic_flag1"];
	vic_val1 = data["vic_val1"];
	vic_flag2 = data["vic_flag2"];
	vic_val2 = data["vic_val2"];

	terrain.platforms.assign(NB_MAX_PLATFORMS, std::vector<int32_t>(terrain.width));
	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		std::memcpy(terrain.platforms[i].data(), &platform_str[i * terrain.width * sizeof(int32_t)], terrain.width * sizeof(int32_t));
	}

	terrain.solid.resize(60);
	for (size_t i = 0; i < 60; i++) {
		terrain.solid[i].assign(solid_walls_str.begin() + i * level_size_8, solid_walls_str.begin() + (i + 1) * level_size_8);
	}

	terrain.bloody.resize(60);
	for (size_t i = 0; i < 60; i++) {
		terrain.bloody[i].assign(bloody_walls_str.begin() + i * level_size_8, bloody_walls_str.begin() + (i + 1) * level_size_8);
	}

	size_t nb_events = data["events"].size();
	for (size_t i = 0; i < nb_events; i++) {
		auto event = data["events"][i];

		int event_id = event["event_id"];
		switch (event_id) {
			case EVENTID_ENEMY: {
				auto event_enemy = std::make_unique<EventEnemy>();

				event_enemy->x_activation = event["x_activation"];
				event_enemy->enemy_id = event["id_ennemi"];
				event_enemy->x = event["x"];
				event_enemy->y = event["y"];
				event_enemy->facing = event["sens"];

				list_event_dormant.push_back(std::move(event_enemy));
				break;
			}

			case EVENTID_ENEMY_GENERATOR: {
				auto event_enemy_gen = std::make_unique<EventEnemyGenerator>();

				event_enemy_gen->x_activation = event["x_activation"];
				event_enemy_gen->enemy_id = event["id_ennemi"];
				event_enemy_gen->x = event["x"];
				event_enemy_gen->y = event["y"];
				event_enemy_gen->facing = event["sens"];
				event_enemy_gen->capacity = event["capacite"];
				event_enemy_gen->period = event["periode"];
				event_enemy_gen->tmp = event["tmp"];

				list_event_dormant.push_back(std::move(event_enemy_gen));
				break;
			}

			case EVENTID_LOCK: {
				auto event_lock = std::make_unique<EventLock>();

				event_lock->x_activation = event["x_activation"];
				event_lock->cond = event["cond"];
				event_lock->flag = event["flag"];
				event_lock->val = event["val"];

				list_event_dormant.push_back(std::move(event_lock));
				break;
			}

			case EVENTID_FORCE_SCROLL: {
				auto event_scroll_speed = std::make_unique<EventScrollSpeed>();

				event_scroll_speed->x_activation = event["x_activation"];
				event_scroll_speed->speed = event["speed"];

				list_event_dormant.push_back(std::move(event_scroll_speed));
				break;
			}

			case EVENTID_FLAG: {
				auto event_set_flag = std::make_unique<EventSetFlag>();

				event_set_flag->x_activation = event["x_activation"];
				event_set_flag->flag = event["flag"];
				event_set_flag->val = event["val"];

				list_event_dormant.push_back(std::move(event_set_flag));
				break;
			}

			case EVENTID_HOLD_FIRE: {
				auto event_hold_fire = std::make_unique<EventHoldFire>();

				event_hold_fire->x_activation = event["x_activation"];
				event_hold_fire->flag = event["flag"];
				event_hold_fire->val = event["val"];

				list_event_dormant.push_back(std::move(event_hold_fire));
				break;
			}

			case EVENTID_BONUS_GENERATOR: {
				auto event_gen_bonus = std::make_unique<EventBonusGenerator>();

				event_gen_bonus->x_activation = event["x_activation"];
				event_gen_bonus->type = event["type"];
				event_gen_bonus->period = event["periode"];

				list_event_dormant.push_back(std::move(event_gen_bonus));
				break;
			}

			case EVENTID_TEXT: {
				auto event_text = std::make_unique<EventText>();

				event_text->x_activation = event["x_activation"];
				event_text->ntxt = event["ntxt"];
				event_text->cond = event["cond"];
				event_text->flag = event["flag"];
				event_text->val = event["val"];

				list_event_dormant.push_back(std::move(event_text));
				break;
			}

			case EVENTID_ANIMATED_BG: {
				auto event_animated_bg = std::make_unique<EventAnimatedBackground>();

				event_animated_bg->x_activation = event["x_activation"];
				event_animated_bg->bg_id = event["id_fond"];
				event_animated_bg->x = event["x"];
				event_animated_bg->y = event["y"];

				list_event_dormant.push_back(std::move(event_animated_bg));
				break;
			}

			case EVENTID_MIDGROUND: {
				auto event_midground = std::make_unique<EventMidground>();

				event_midground->x_activation = event["x_activation"];
				event_midground->pic = event["pic"];
				event_midground->x = event["x"];
				event_midground->y = event["y"];

				list_event_dormant.push_back(std::move(event_midground));
				break;
			}

			case EVENTID_FOREGROUND: {
				auto event_foreground = std::make_unique<EventForeground>();

				event_foreground->pic = event["pic"];
				event_foreground->x_activation = event["x_activation"];
				event_foreground->x = event["x"];
				event_foreground->y = event["y"];

				list_event_dormant.push_back(std::move(event_foreground));
				break;
			}

			case EVENTID_RPG: {
				auto event_rpg = std::make_unique<EventRPG>();

				event_rpg->x_activation = event["x_activation"];
				event_rpg->num = event["num"];
				event_rpg->cond = event["cond"];
				event_rpg->flag = event["flag"];
				event_rpg->val = event["val"];

				list_event_dormant.push_back(std::move(event_rpg));
				break;
			}

			case EVENTID_MUSIC: {
				auto event_music = std::make_unique<EventMusic>();

				event_music->x_activation = event["x_activation"];
				event_music->id = event["id"];
				event_music->play = event["play"];

				list_event_dormant.push_back(std::move(event_music));
				break;
			}

			case EVENTID_WEATHER: {
				auto event_weather = std::make_unique<EventWeather>();

				event_weather->x_activation = event["x_activation"];
				event_weather->intensity = event["intensite"];
				event_weather->type = event["type"];

				list_event_dormant.push_back(std::move(event_weather));
				break;
			}

			case EVENTID_BONUS: {
				auto event_bonus = std::make_unique<EventBonus>();
				event_bonus->x_activation = event["x_activation"];
				event_bonus->type = event["type"];
				event_bonus->x = event["x"];
				event_bonus->y = event["y"];

				list_event_dormant.push_back(std::move(event_bonus));
				break;
			}

			case EVENTID_TURRET: {
				auto event_vehicle = std::make_unique<EventVehicle>();
				event_vehicle->x_activation = event["x_activation"];
				event_vehicle->vehicle_id = event["id_vehicule"];
				event_vehicle->x = event["x"];
				event_vehicle->y = event["y"];
				event_vehicle->dir = event["dir"];

				list_event_dormant.push_back(std::move(event_vehicle));
				break;
			}

			case EVENTID_SOUND: {
				auto event_sound = std::make_unique<EventSound>();
				event_sound->x_activation = event["x_activation"];
				event_sound->nsnd = event["nsnd"];

				list_event_dormant.push_back(std::move(event_sound));
				break;
			}
			default:
				// A bad level file would otherwise lose the event silently
				debug << "Unknown event_id " << event_id << "\n";
				break;
		}
	}

	debug << "Successfully loaded all level files\n";

	return true;
}

void Game::releaseLevel() {
	world_.resetLevel();
	list_event_dormant.clear();
	list_event.clear();
	list_impacts.clear();
	list_weather.clear();
	list_bubbles.clear();
}

void Game::updateAll() {
	Entities& entities = level().entities;
	session().phase = !session().phase;

	if (session().phase) slow_phase = !slow_phase;

	level().rules.bonus_ok = false;

	if (player1 != NULL) {
		level().rules.bonus_ok = level().rules.bonus_ok || player1->okBonus();
	}

	if (player2 != NULL) {
		level().rules.bonus_ok = level().rules.bonus_ok || player2->okBonus();
	}

	if (!level().camera.frozen) level().camera.follow(session().players, false, (int)level().terrain.width);

	updateEvents();

	// Scroll again so locks set by the events apply
	if (!level().camera.frozen) level().camera.follow(session().players, true, (int)level().terrain.width);

	updateCheat();

	if (level().rules.flags[FLAG_BUBBLES]) updateBubbles();

	if (level().rules.weather_type == WEATHER_RAIN || level().rules.weather_type == WEATHER_SNOW) updateWeather();

	UpdateCollection(entities.static_bgs);
	UpdateCollection(entities.animated_bgs);
	UpdateCollection(entities.moving_platforms);
	UpdateCollection(entities.vehicles);
	UpdateCollection(session().players);
	UpdateCollection(entities.bb_shots);
	UpdateCollection(entities.cows);
	UpdateCollection(list_impacts);
	// Before the splatters, a splatter follows its enemy and must see it destroyed in the same frame
	UpdateCollection(entities.enemies);
	UpdateCollection(entities.gore);
	UpdateCollection(entities.enemy_shots);
	UpdateCollection(entities.splatters);
	UpdateCollection(entities.bonus);
	UpdateCollection(entities.foregrounds);

	UpdateCollection(entities.cool_texts);
	UpdateCollection(entities.enemy_gens);
	UpdateCollection(entities.bonus_gens);
	updateLock();
	updateHoldFire();
	updateTargetedPlayer();
	updateFlags();
	updateGoArrow();

	if (level().rules.weather_type == WEATHER_DISTORT && level().rules.weather_intensity != 0) updateDeformation();

	manageCollisions();

	cleanLists();
}

void Game::drawAll(bool flip) {
	Entities& entities = level().entities;
	if (!flip) {
		session().phase = false;
		slow_phase = false;
	}

	fps_current_count += 1;
	static Uint32 fps_start = SDL_GetTicks();
	if (SDL_GetTicks() - fps_start >= 1000) {
		fps_count = fps_current_count;
		fps_current_count = 0;
		fps_start = SDL_GetTicks();
	}

	world_.scroll.draw(level().camera, level().terrain);
	DrawCollection(entities.static_bgs);
	DrawCollection(entities.animated_bgs);

	if (level().rules.flags[FLAG_BUBBLES]) {
		DrawCollection(list_bubbles);
	}

	DrawCollection(entities.moving_platforms);
	DrawCollection(entities.splatters);
	DrawCollection(entities.gore);

	DrawCollection(entities.enemies);
	DrawCollection(entities.enemy_shots);
	DrawCollection(entities.bonus);
	DrawCollection(entities.bb_shots);
	DrawCollection(session().players);
	DrawCollection(entities.vehicles);
	DrawCollection(list_impacts);
	DrawCollection(entities.cows);
	DrawCollection(list_weather);
	DrawCollection(entities.foregrounds);

	if (level().rules.weather_type == WEATHER_DISTORT && level().rules.weather_intensity != 0) drawDeformation();

	drawHUB();
	drawTimer();
	go_.Draw();
	DrawCollection(entities.cool_texts);

	drawDebugInfos();

	if (flip) {
		graphics.Flip();
	}
}

Task<> Game::gameLoop() {
	int n_updates = update_regulator_.Step();
	// A dialog starts as soon as an event asks for it, like it did inside updateAll
	for (; n_updates > 0 && session().rpg_to_play == -1; --n_updates) {
		updateAll();
	}

	co_await updateMenu();
	co_await updateRPG();

	drawAll();
}

void Game::releaseGame() { session().players.clear(); }

bool Game::loadGame() {
	debug << "---------------------------------------------------------------\n";
	debug << "Loading global game files...\n";
	debug << "---------------------------------------------------------------\n";

	if (!assets().pics.blip.loadGFX("blip.gfx")) return false;

	debug << "Successfully loaded <blip.gfx>\n";

	if (!assets().pics.blop.loadGFX("blop.gfx")) return false;

	debug << "Successfully loaded <blop.gfx>\n";

	if (!assets().pics.bb.loadGFX("bb.gfx")) return false;

	debug << "Successfully loaded <bb.gfx>\n";

	if (!assets().pics.misc.loadGFX("misc.gfx")) return false;

	debug << "Successfully loaded <misc.gfx>\n";

	if (!assets().pics.rpg_bb.loadGFX("rpg_bb.gfx")) return false;

	debug << "Successfully loaded <rpg_bb.gfx>\n";

	if (!assets().fonts.score_blip.load("scorei.lft")) return false;

	debug << "Successfully loaded <scorei.lft>\n";

	if (!assets().fonts.score_blop.load("scoreo.lft")) return false;

	debug << "Successfully loaded <scoreo.lft>\n";

	if (!assets().fonts.ammo.load("ammo1.lft")) return false;

	debug << "Successfully loaded <ammo1.lft>\n";

	if (!assets().fonts.ammo_used.load("ammo2.lft")) return false;

	debug << "Successfully loaded <ammo2.lft>\n";

	if (!assets().sounds.misc.loadSFX("misc.sfx")) return false;

	debug << "Successfully loaded <misc.sfx>\n";

	if (!assets().sounds.bb.loadSFX("bb.sfx")) return false;

	debug << "Successfully loaded <bb.sfx>\n";

	if (!loadList("assets/bb.lst.json")) return false;

	debug << "Successfully loaded <bb.lst>\n";
	debug << "Successfully loaded all needed global game files\n";

	return true;
}

template <class T>
void Game::DrawCollection(const T& xs) {
	for (auto& pl : xs) {
		pl->render();
	}
}

template <class T>
void Game::RemoveDestroyed(T& xs) {
	xs.erase(std::remove_if(xs.begin(), xs.end(), [](auto& x) { return x->isDestroyed(); }), xs.end());
}

void Game::cleanLists() {
	Entities& entities = level().entities;
	RemoveDestroyed(session().players);
	RemoveDestroyed(entities.static_bgs);
	RemoveDestroyed(entities.bb_shots);
	RemoveDestroyed(entities.cows);
	RemoveDestroyed(list_bubbles);
	RemoveDestroyed(list_impacts);
	RemoveDestroyed(entities.enemies);
	RemoveDestroyed(entities.bonus);
	RemoveDestroyed(entities.enemy_gens);
	RemoveDestroyed(entities.bonus_gens);
	RemoveDestroyed(entities.cool_texts);
	RemoveDestroyed(entities.animated_bgs);
	RemoveDestroyed(entities.foregrounds);
	RemoveDestroyed(entities.splatters);
	RemoveDestroyed(entities.enemy_shots);
	RemoveDestroyed(list_weather);
	RemoveDestroyed(entities.gore);
	RemoveDestroyed(entities.moving_platforms);
}

void Game::updateEvents() {
	// Sleeping events move to the pending list when they are about to fire
	for (auto it = list_event_dormant.rbegin(); it != list_event_dormant.rend(); ++it) {
		auto& event = *it;
		if (!event->shouldWake()) {
			break;
		}
		list_event.push_back(std::move(event));
	}

	list_event_dormant.erase(std::remove_if(list_event_dormant.begin(), list_event_dormant.end(), [](auto& ev) { return !ev.get(); }),
							 list_event_dormant.end());

	for (auto& event : list_event) {
		if (event->shouldActivate()) {
			event->doEvent();
			event.reset(nullptr);
		}
	}
	list_event.erase(std::remove_if(list_event.begin(), list_event.end(), [](auto& ev) { return !ev.get(); }), list_event.end());
}

void Game::manageCollisions() {
	Entities& entities = level().entities;
	for (Shot* shot : entities.bb_shots) {
		for (auto& enemy : entities.enemies) {
			if (shot->collision(enemy.get())) {
				enemy->takeHit(shot);
			}
		}
	}

	for (auto& shot : entities.cows) {
		for (auto& enemy : entities.enemies) {
			if (shot->collision(enemy.get())) {
				enemy->takeHit(shot.get());
			}
		}
	}

	for (auto& bonus : entities.bonus) {
		for (Ball* ball : session().players) {
			if (bonus->collision(ball)) {
				bonus->onPickup(ball);
			}
		}
	}

	if (wait_for_victory <= 0) {
		for (auto& enemy : entities.enemies) {
			for (Ball* hero : session().players) {
				if (enemy->collision(hero)) hero->takeHit(enemy->damage());
			}
		}

		for (auto& shot : entities.enemy_shots) {
			for (Ball* hero : session().players) {
				if (shot->collision(hero)) {
					hero->takeHit(shot->damage());
				}
			}
		}
	}
}

void Game::updateLock() {
	Camera::Lock& lock = level().camera.lock;
	if (lock.on && lock.released(level().entities, level().rules.flags)) {
		lock.on = false;
		go_.Come();
	}
}

void Game::updateHoldFire() {
	LevelRules& rules = level().rules;
	if (rules.hold_fire.on && rules.hold_fire.released(rules.flags)) rules.hold_fire.on = false;
}

void Game::drawHUB() {
	if (player1 != NULL && player1->nb_life > 0) {
		auto color = player1->id_ball == ID_BLIP ? HUD::Color::Blue : HUD::Color::Orange;
		hud_.Draw(player1.get(), HUD::Location::Left, color, bonus_level);
	}

	if (player2 != NULL && player2->nb_life > 0) {
		auto color = player2->id_ball == ID_BLIP ? HUD::Color::Blue : HUD::Color::Orange;
		hud_.Draw(player2.get(), HUD::Location::Right, color, bonus_level);
	}
}

void Game::drawHUBhp(int x, int y, int hp) {
	static const int x_hp[] = {3, 0, 13, 44, 31};
	static const int y_hp[] = {0, 21, 48, 21, 0};

	for (int i = 0; i < hp; i++) assets().pics.bb.anim("health_full")[4 - i]->BlitTo(graphics.back(), x + x_hp[i], y + y_hp[i]);

	for (int i = hp; i < 5; i++) assets().pics.bb.anim("health_empty")[4 - i]->BlitTo(graphics.back(), x + x_hp[i], y + y_hp[i]);
}

void Game::drawDebugInfos() {
	if (show_fps) {
		assets().fonts.rpg.print(graphics.back(), 10, 130, ("FPS = " + std::to_string(fps_count)).c_str());
	}
}

void Game::updateTargetedPlayer() {
	if (session().players.empty()) {
		dummy_player_.x = level().camera.offset + 320;
		dummy_player_.y = level().terrain.platforms[0][level().camera.offset + 320];
		session().targeted_player = &dummy_player_;
		return;
	}

	targeted_idx_ += 1;
	targeted_idx_ %= session().players.size();
	session().targeted_player = session().players[targeted_idx_];
}

Task<> Game::updateRPG() {
	if (session().rpg_to_play == -1) co_return;

	bool continued = true;

	rpg.startPlay(session().rpg_to_play);

	while (continued && !in.quitRequested()) {
		drawAll(false);
		continued = rpg.drawScene(graphics.back());
		graphics.Flip();

		co_await nextFrame();
	}

	rpg.stopPlay();
	co_await in.waitClean();
	session().rpg_to_play = -1;
	update_regulator_.Skip();
}

void Game::updateVictoryAndDefeat() {
	if (session().players.empty()) {
		wait_for_death += 1;

		if (wait_for_death >= 200) players_dead = true;

		return;
	}

	if (level().camera.offset >= vic_x && level().rules.flags[vic_flag1] == vic_val1 && level().rules.flags[vic_flag2] == vic_val2) {
		level().rules.hold_fire.on = true;
		wait_for_victory += 1;

		if (level().rules.flags[1] == 999) wait_for_victory = 200;

		if (wait_for_victory >= 200) level_done = true;
	} else {
		if (wait_for_victory > 0) wait_for_victory = 0;
	}
}

void Game::updateFlags() {
	level().rules.flags[FLAG_NB_GEN] = (int)level().entities.enemy_gens.size();
	level().rules.flags[FLAG_NB_ENEMIES] = (int)level().entities.enemies.size();
	level().rules.bonus_mode = level().rules.flags[FLAG_BONUS];

	if (level().rules.flags[FLAG_TIMER] > 0 && wait_for_victory == -1) {
		timer_tick += 1;
		timer_tick %= 80;

		if (timer_tick == 0) {
			level().rules.flags[FLAG_TIMER] -= 1;

			if (level().rules.flags[FLAG_TIMER] == 0 && bonus_level) level_done = true;
		}
	}
}

void Game::drawTimer() {
	if (level().rules.flags[FLAG_TIMER] > 0) {
		assets().fonts.cool.printC(graphics.back(), 320, 20, texts()[TXT_TIME]);
		assets().fonts.cool.printC(graphics.back(), 320, 50, std::to_string(level().rules.flags[FLAG_TIMER]));
	}
}

Task<> Game::updateMenu() {
	if (in.scanKey(DIK_ESCAPE)) {
		MenuGame menu;
		int r;
		debug << "Pause menu\n";

		drawAll(false);
		graphics.Flip();
		drawAll(false);
		graphics.Flip();

		menu.Draw(graphics.back());
		graphics.Flip();

		co_await in.waitClean();

		while (!(r = menu.Update()) && !in.quitRequested()) {
			drawAll(false);
			menu.Draw(graphics.back());

			graphics.Flip();

			co_await nextFrame();
		}

		// The key that closed the menu must not reach the players
		co_await in.waitClean();

		if (r == 2) skipped = true;

		drawAll(false);
		graphics.Flip();
		drawAll(false);
		graphics.Flip();

		update_regulator_.Skip();
	}
}

void Game::updateCheat() {
	if (wait_cheat < 20) wait_cheat += 1;

	if (!settings().cheat) return;

	if (player1 != NULL) {
		if (in.scanKey(DIK_F1)) player1->ammo[player1->weapon_id] += 10;

		if (wait_cheat < 20) return;

		if (in.scanKey(DIK_F3)) {
			player1->nb_life++;
			wait_cheat = 0;
		}

		if (in.scanKey(DIK_F2)) {
			player1->nb_cow_bomb += 1;
			wait_cheat = 0;
		}
	}

	if (player2 != NULL) {
		if (in.scanKey(DIK_F5)) player2->ammo[player2->weapon_id] += 10;

		if (wait_cheat < 20) return;

		if (in.scanKey(DIK_F7)) {
			player2->nb_life++;
			wait_cheat = 0;
		}

		if (in.scanKey(DIK_F6)) {
			player2->nb_cow_bomb += 1;
			wait_cheat = 0;
		}
	}

	if (in.scanKey(DIK_F9)) level_done = true;

	if (wait_cheat < 20) return;

	if (in.scanKey(DIK_F12)) {
		show_fps = !show_fps;
		wait_cheat = 0;
	}

	if (in.scanKey(DIK_F11)) {
		show_lists = !show_lists;
		wait_cheat = 0;
	}
}

namespace {
struct PlayerTotals {
	Ball* player = nullptr;
	Font* fnt = nullptr;
	int xbase = 0;
	int calc = 4;
	int perfect = 0, c_perfect = 0;
	int obj = 0, c_obj = 0;
	int killed = 0, c_killed = 0;
	int c_time = 0;
	int total_bonus = 0, total = 0;
	int life_up = 0;

	void add(int& counter, int step, int points) {
		counter += step;
		total_bonus += points;
		total += points;
	}

	void step(int timer) {
		if (calc == 0) {
			if (c_perfect >= perfect)
				calc = 1;
			else
				add(c_perfect, 1000, 1000);
		} else if (calc == 1) {
			if (c_obj >= obj)
				calc = 2;
			else
				add(c_obj, 1000, 1000);
		}
		if (calc == 2) {
			if (c_killed >= killed)
				calc = 3;
			else
				add(c_killed, 1, 500);
		} else if (calc == 3) {
			if (c_time >= timer)
				calc = 4;
			else
				add(c_time, 1, 1000);
		}
	}

	void draw(int timer) {
		if (life_up > 0) assets().fonts.rpg.printC(graphics.back(), xbase + 105, 270, "LIFE UP");

		auto line = [&](int y, const char* label, int value) {
			assets().fonts.rpg.print(graphics.back(), xbase, y, label);
			fnt->printR(graphics.back(), xbase + 210, y, std::to_string(value).c_str());
		};
		if (perfect > 0) line(180, "PERFECT", c_perfect);
		if (obj > 0) line(200, "Objective", c_obj);
		line(220, "Casualties", c_killed);
		assets().fonts.rpg.printR(graphics.back(), xbase + 220, 220, "%");
		if (timer > 0) line(240, "Time left", c_time);
		line(300, "BONUS", total_bonus);
		line(320, "SCORE", total);

		if (total / 500000 > player->mod_life) {
			player->mod_life = total / 500000;
			player->nb_life++;
			life_up = 100;
		}
	}
};
}  // namespace

Task<> Game::showPE(bool bonus, bool bonus_failed) {
	PlayerTotals totals[2];
	totals[0].player = player1.get();
	totals[1].player = player2.get();

	graphics.spare()->BltFast(0, 0, graphics.back(), NULL);

	Rect r;
	r.top = 100;
	r.left = 60;
	r.right = 580;
	r.bottom = 400;
	halfTone(graphics.spare(), &r);

	assets().music.inter.play(0);

	if (session().players.size() == 2) {
		totals[0].calc = totals[1].calc = 0;
		totals[0].xbase = 80;
		totals[1].xbase = 350;
	} else if (player1 != NULL && player1->nb_life > 0) {
		totals[0].calc = 0;
		totals[0].xbase = 210;
	} else if (player2 != NULL) {
		totals[1].calc = 0;
		totals[1].xbase = 210;
	}

	for (auto& t : totals) {
		if (t.calc == 4) continue;
		t.fnt = t.player->id_ball == ID_BLIP ? &assets().fonts.score_blip : &assets().fonts.score_blop;
		if (session().enemies_created > 0) t.killed = (100 * t.player->getKilled()) / session().enemies_created;
		if (!bonus || (level().rules.flags[FLAG_TIMER] > 0 && !players_dead)) t.obj = 50000;
		if (t.obj > 0 && t.player->perfect) t.perfect = 50000;
		t.total = t.player->getScore();
		if (bonus_failed) t.perfect = t.obj = t.killed = 0;
	}
	if (bonus_failed) level().rules.flags[FLAG_TIMER] = 0;

	int tally_tick = 0;

	// Runs at least once so the screen shows even when no player is left to tally
	do {
		tally_tick += 1;
		tally_tick %= 3;

		if (in.anyKeyPressed()) tally_tick = 0;

		if (tally_tick == 0) {
			for (auto& t : totals) t.step(level().rules.flags[FLAG_TIMER]);
		}

		graphics.back()->BltFast(0, 0, graphics.spare(), NULL);

		assets().fonts.cool.printC(graphics.back(), 320, 120, bonus_failed ? "BONUS STAGE FAILED!" : "LEVEL COMPLETE!");

		for (auto& t : totals) {
			if (t.fnt) t.draw(level().rules.flags[FLAG_TIMER]);
		}

		graphics.Flip();

		co_await nextFrame();
	} while (!in.quitRequested() && (totals[0].calc < 4 || totals[1].calc < 4));

	if (!in.quitRequested()) {
		co_await in.waitClean();
		co_await in.waitKey();
	}

	for (auto& t : totals) {
		if (t.fnt) t.player->setScore(t.total);
	}

	assets().music.inter.stop();
}

void Game::updateWeather() {
	for (auto& pl : list_weather) {
		pl->update();
	}

	while (list_weather.size() < (size_t)level().rules.weather_intensity) {
		if (level().rules.weather_type == WEATHER_SNOW) {
			next_snowflake = (next_snowflake + 1) % NB_SNOWFLAKES;
			WeatherSnow* snowflake = &snow[next_snowflake];

			int d = rnd() % 3;

			snowflake->destroyed = false;
			snowflake->xbase = level().camera.offset + rnd() % 800;
			snowflake->y = rnd() % 480 - 500;

			if (d <= 1)
				snowflake->pic = assets().pics.misc.pic("snowflake_small");
			else
				snowflake->pic = assets().pics.misc.pic("snowflake_large");

			snowflake->dy = level().rules.weather_intensity / 20 + d;
			snowflake->phi = rnd() % 360;
			snowflake->xwide = 10 + d * 4;

			if (level().terrain.solid_wall(snowflake->xbase, 0)) snowflake->y -= 550;

			list_weather.emplace_back(snowflake);
		} else if (level().rules.weather_type == WEATHER_RAIN) {
			next_raindrop = (next_raindrop + 1) % NB_RAINDROPS;
			WeatherRain* raindrop = &rain[next_raindrop];

			int d = rnd() % 4;

			raindrop->destroyed = false;
			raindrop->x = level().camera.offset + rnd() % 800;
			raindrop->y = rnd() % 480 - 480;
			raindrop->pic = assets().pics.misc.anim("raindrop")[d];
			raindrop->dy = 7 + d * 2;

			if (level().terrain.solid_wall(raindrop->x, 0)) raindrop->y -= 550;

			list_weather.emplace_back(raindrop);
		}
	}
}

template <class T>
void Game::UpdateCollection(const T& xs) {
	for (auto& pl : xs) {
		pl->update();
	}
}

void Game::updateGoArrow() {
	go_.Update();
	if (last_x_go_ != level().camera.offset) {
		last_x_go_ = level().camera.offset;
		go_.Leave();
	}
}

void Game::updateDeformation() {
	phi_deform += 2;
	phi_deform %= 360;
}

void Game::drawDeformation() {
	Rect r;
	int stride = 20;
	int phi = phi_deform;
	int dphi = 10;
	int x;
	int xt;

	stride = 2;
	dphi = 1;

	for (int y = 0; y < 480; y += stride) {
		phi += dphi;
		phi %= 360;

		r.top = y;
		r.bottom = y + stride;

		x = xt = int(5 * std::sin(phi * std::numbers::pi / 180));

		if (x < 0) {
			r.left = -x;
			r.right = 640;
			x = 0;
		} else {
			r.left = 0;
			r.right = 640 - x;
		}

		graphics.back()->BltFast(x, y, graphics.back(), &r);

		if (xt < 0) {
			r.left = 640 + xt;
			r.right = 640;
		} else {
			r.left = 0;
			r.right = xt;
		}

		graphics.back()->FillRect(&r, 0);
	}
}

void Game::updateBubbles() {
	for (Ball* pl : session().players) {
		spawnBubble(pl);
	}

	for (auto& pl : level().entities.enemies) {
		if (list_bubbles.size() < 15) {
			break;
		}
		spawnBubble(pl.get());
	}

	for (auto& pl : list_bubbles) {
		pl->update();
	}
}

void Game::spawnBubble(Sprite* s) {
	s->wait_bubble += rnd() % 10;

	if (s->wait_bubble > 300) {
		Bubble* b = new Bubble();

		b->xbase = s->x;
		b->y = s->y - 10;
		b->pic = assets().pics.misc.anim("bubble")[rnd() % 3];
		b->dphi = 5 + rnd() % 3;
		b->dy = -(2 + rnd() % 2);

		list_bubbles.emplace_back(b);

		s->wait_bubble = 0;
	}
}

bool Game::loadList(const char* file_path) {
	std::ifstream input(file_path);
	if (!input.good()) {
		debug << "Cannot open " << file_path << "\n";
		return false;
	}

	json data = json::parse(input);

	nb_part = 0;
	for (auto item : data["items"]) {
		if (item["type"] == "BRIEFING") {
			type_part[nb_part] = PART_BRIEFING;
			type_lvl[nb_part] = 0;
		} else if (item["type"] == "LEVEL") {
			type_part[nb_part] = PART_LEVEL;
			type_lvl[nb_part] = item["level"];
		} else {
			assert(false);
		}

		file_names[nb_part] = item["path"].get<std::string>();

		nb_part++;
	}

	return true;
}

void Game::drawLoading() { assets().fonts.cool.printC(graphics.back(), 320, 205, "LOADING"); }

Task<> Game::getHiscore() {
	if (player1 != NULL && hiScores().isGood(player1->getScore())) co_await getName(player1.get(), 1);

	if (player2 != NULL && hiScores().isGood(player2->getScore())) co_await getName(player2.get(), 2);
}

Task<> Game::getName(Player* hero, int player_num) {
	const std::string title = "PLAYER " + std::to_string(player_num) + " GOT A HIGH SCORE!";
	std::string name;
	int key = -1;
	int x = 640;

	while (!in.quitRequested() && x >= 40) {
		x -= 20;

		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);

		assets().fonts.menu.printC(graphics.back(), 320 - x, 160, title);
		assets().fonts.menu.printC(graphics.back(), 320 + x, 210, "PLEASE ENTER YOUR NAME :");

		graphics.Flip();

		co_await nextFrame();
	}

	while (!in.quitRequested() && key != DIK_RETURN) {
		co_await in.waitClean();
		key = co_await in.waitKey();
		if (in.scanKey(DIK_RETURN)) key = DIK_RETURN;

		std::string key_tmp = DIK_to_string(key, settings().lang == LANG_FR);

		if (key == DIK_SPACE && name.size() < 19) name += ' ';
		if (key == DIK_BACKSPACE && !name.empty()) {
			name.pop_back();
		} else if (key_tmp.size() == 1 && name.size() < 19) {
			name += key_tmp[0];
		}

		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);
		assets().fonts.menu.printC(graphics.back(), 320 - x, 160, title);
		assets().fonts.menu.printC(graphics.back(), 320 + x, 210, "PLEASE ENTER YOUR NAME :");
		assets().fonts.menu.printC(graphics.back(), 320, 260, name);
		graphics.Flip();

		co_await nextFrame();
	}

	while (!in.quitRequested() && x < 640) {
		x += 20;

		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);

		assets().fonts.menu.printC(graphics.back(), 320 - x, 160, title);
		assets().fonts.menu.printC(graphics.back(), 320 + x, 210, "PLEASE ENTER YOUR NAME :");
		assets().fonts.menu.printC(graphics.back(), 320, 260 + x, name);

		graphics.Flip();

		co_await nextFrame();
	}

	hiScores().add(hero->getScore(), name);
	App::current().save();
}

Task<> Game::showGameOver() {
	Chrono t;

	assets().music.inter.play(1);

	while (!in.quitRequested() && t.elapsed() <= 5000 && !in.anyKeyPressed()) {
		assets().pics.inter.pic("game_over")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();

		co_await nextFrame();
	}
	assets().music.inter.stop();
}

Task<> Game::showHighScores() {
	int x[HS_NB_SCORES];

	for (int i = 0; i < HS_NB_SCORES; i++) x[i] = 400 + 160 * i;

	Chrono scores_timer;
	while (!in.quitRequested() && !in.anyKeyPressed() && (scores_timer.elapsed() <= 10000)) {
		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);

		for (int i = 0; i < HS_NB_SCORES; i++) {
			if (x[i] > 0) x[i] -= 20;

			assets().fonts.cool.printR(graphics.back(), 300 - x[i], 160 + 30 * i, std::to_string(hiScores().getScore(i)).c_str());

			assets().fonts.cool.print(graphics.back(), 320 + x[i], 160 + 30 * i, hiScores().getName(i));
		}

		graphics.Flip();
		co_await nextFrame();
	}

	while (!in.quitRequested() && x[HS_NB_SCORES - 1] < 350) {
		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);

		for (int i = 0; i < HS_NB_SCORES; i++) {
			if (i == 0 || x[i - 1] >= 160) x[i] += 20;

			assets().fonts.cool.printR(graphics.back(), 300 - x[i], 160 + 30 * i, std::to_string(hiScores().getScore(i)).c_str());

			assets().fonts.cool.print(graphics.back(), 320 + x[i], 160 + 30 * i, hiScores().getName(i));
		}

		graphics.Flip();
		co_await nextFrame();
	}

	co_await in.waitClean();
}

Task<> Game::go() {
	TitleScreen menu;
	int r;

	{
		graphics.back()->FillRect(nullptr, 0);
		assets().fonts.cool.printC(graphics.back(), 320, 240, "PLEASE WAIT");
	}

	CINEPlayer cine;

	cine.loadPBK("intro.gfx");
	assets().music.interl.play(0);
	co_await cine.playScene("intro.cin", graphics.back(), true);

	debug << "Title menu\n";
	co_await in.waitClean();

	Chrono diff_start;

	do {
		r = RET_CONTINUE;

		while (!r && !in.quitRequested()) {
			if (in.anyKeyPressed()) {
				diff_start.Reset();
			}

			if (diff_start.elapsed() >= 10000) {
				switch (attract_step_) {
					case 0:
					case 2:
					case 4:
					case 6:
						co_await showMainScreen();
						break;

					case 1:
						co_await showHighScores();
						break;

					case 5:
						co_await showCredits();
						break;

					case 3:
					case 7:
						cine.loadPBK("intro.gfx");
						co_await cine.playScene("intro.cin", graphics.back(), true);
						break;
				}

				attract_step_ = (attract_step_ + 1) % 8;

				if (in.anyKeyPressed()) {
					diff_start.Reset();
					co_await in.waitClean();
				}
			} else {
				assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);

				r = menu.update();
				menu.draw(graphics.back());

				graphics.Flip();
			}

			co_await nextFrame();
		}

		if (r == RET_HISCORES) {
			// The confirming key would end the table at once
			co_await in.waitClean();
			co_await showHighScores();
			diff_start.Reset();
		} else if (r == RET_START_GAME) {
			JoinScreen join;

			auto joined = JoinScreen::Output::Continue;
			while (joined == JoinScreen::Output::Continue && !in.quitRequested()) {
				joined = join.update();
				join.draw();
				graphics.Flip();

				co_await nextFrame();
			}

			if (joined == JoinScreen::Output::Start) {
				for (int i = 0; i < join.players(); i++) controllers_[i] = join.player(i);

				CharacterSelection select;

				auto selected = CharacterSelection::Output::Continue;
				while (selected == CharacterSelection::Output::Continue && !in.quitRequested()) {
					selected = select.update();
					select.draw();
					graphics.Flip();

					co_await nextFrame();
				}
				// The confirming pad A is also the default jump
				co_await in.waitClean();

				drawLoading();
				graphics.Flip();

				co_await playGame(join.players(), int(selected));
				assets().music.interl.play(0);
			}

			diff_start.Reset();
		}

		co_await nextFrame();

	} while (r != RET_EXIT && !in.quitRequested());

	assets().music.interl.stop();
}

void Game::showBriefing(const char* fn) {
	pbk_briefing.loadGFX(fn);

	briefing = true;

	Rect r;

	r.left = 0;
	r.right = 640;
	r.top = 0;
	r.bottom = 480;

	graphics.back()->FillRect(&r, 0);

	pbk_briefing.pic("briefing")->PasteTo(graphics.back(), 0, 0);
	assets().fonts.rpg.printC(graphics.back(), 320, 460, "Loading...");
	graphics.Flip();
}

Task<> Game::showMainScreen() {
	Chrono t;

	while (!in.anyKeyPressed() && !in.quitRequested() && t.elapsed() <= 5000) {
		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();

		co_await nextFrame();
	}
}

Task<> Game::showCredits(bool theEnd) {
	static const int LINE_GAP = 20;
	static const int TITLE_GAP = 40;
	static const int SECTION_GAP = 120;
	static const int SSPEED = 2;

	int npage = 0;
	Chrono t;
	int y = 480;
	int ey = 0;
	int last_y = 100;
	int xcred = 320;

	PictureBank pbk_cred;

	if (theEnd) {
		xcred = 510;
		pbk_cred.loadGFX("credits.gfx");
	}

	update_regulator_.Skip();

	while (!in.quitRequested() && (!in.anyKeyPressed() || theEnd) && last_y > 0) {
		int n_updates = update_regulator_.Step();
		for (; n_updates > 0; --n_updates) {
			ey = (ey + 1) % SSPEED;
			if (ey == 0) {
				y--;
			}
		}

		{
			Rect r;

			r.left = 0;
			r.right = 640;
			r.top = 0;
			r.bottom = 480;

			graphics.back()->FillRect(&r, 0);
		}

		if (theEnd) {
			pbk_cred.anim("pages")[npage]->PasteTo(graphics.back(), 0, 0);
		} else {
			assets().pics.inter.pic("title")->BlitTo(graphics.back(), 0, 0);
		}

		assets().fonts.rpg.printC(graphics.back(), xcred, y + LINE_GAP, "Credits");

		int y1 = y + LINE_GAP + SECTION_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y1, "CODE");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 0, "Benjamin Karaban");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 1, "Sylvain Bugat");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 3, "2019 REVAMP");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 4, "Guillaume Sanchez");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 6, "Web port");
		assets().fonts.rpg.printC(graphics.back(), xcred, y1 + TITLE_GAP + LINE_GAP * 7, "Adam Kecskes");

		int y2 = y1 + 4 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y2, "MUSIC AND SOUND");
		assets().fonts.rpg.printC(graphics.back(), xcred, y2 + LINE_GAP, "DESIGN");
		assets().fonts.rpg.printC(graphics.back(), xcred, y2 + TITLE_GAP + LINE_GAP, "Gerard Kelly");

		int y3 = y2 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y3, "ADDITIONAL MUSIC");
		assets().fonts.rpg.printC(graphics.back(), xcred, y3 + TITLE_GAP + 0 * LINE_GAP, "El Mobo");
		assets().fonts.rpg.printC(graphics.back(), xcred, y3 + TITLE_GAP + 1 * LINE_GAP, "Rik Ede");

		int y4 = y3 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y4, "ARTWORK");
		assets().fonts.rpg.printC(graphics.back(), xcred, y4 + TITLE_GAP + LINE_GAP * 0, "Laurent Schneider");
		assets().fonts.rpg.printC(graphics.back(), xcred, y4 + TITLE_GAP + LINE_GAP * 1, "Jérémie Comarmond");
		assets().fonts.rpg.printC(graphics.back(), xcred, y4 + TITLE_GAP + LINE_GAP * 2, "Jérôme Karaban");
		assets().fonts.rpg.printC(graphics.back(), xcred, y4 + TITLE_GAP + LINE_GAP * 3, "Didier Colin");
		assets().fonts.rpg.printC(graphics.back(), xcred, y4 + TITLE_GAP + LINE_GAP * 4, "Sylvain Bugat");

		int y5 = y4 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y5, "ADDITIONAL SOUND");
		assets().fonts.rpg.printC(graphics.back(), xcred, y5 + LINE_GAP, "AND VOCAL EFFECTS");
		assets().fonts.rpg.printC(graphics.back(), xcred, y5 + TITLE_GAP + 1 * LINE_GAP, "Eric Khodja");
		assets().fonts.rpg.printC(graphics.back(), xcred, y5 + TITLE_GAP + 2 * LINE_GAP, "Jérôme Karaban");
		assets().fonts.rpg.printC(graphics.back(), xcred, y5 + TITLE_GAP + 3 * LINE_GAP, "Benjamin Karaban");
		assets().fonts.rpg.printC(graphics.back(), xcred, y5 + TITLE_GAP + 4 * LINE_GAP, "Vincent Lagarrigue");

		int y6 = y5 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y6, "STORYLINE");
		assets().fonts.rpg.printC(graphics.back(), xcred, y6 + TITLE_GAP + 0 * LINE_GAP, "Benjamin Karaban");
		assets().fonts.rpg.printC(graphics.back(), xcred, y6 + TITLE_GAP + 1 * LINE_GAP, "Eric Khodja");

		int y7 = y6 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y7, "ORIGINAL CONCEPT");
		assets().fonts.rpg.printC(graphics.back(), xcred, y7 + TITLE_GAP + 0 * LINE_GAP, "Laurent Schneider");
		assets().fonts.rpg.printC(graphics.back(), xcred, y7 + TITLE_GAP + 1 * LINE_GAP, "Jérémie Comarmond");
		assets().fonts.rpg.printC(graphics.back(), xcred, y7 + TITLE_GAP + 2 * LINE_GAP, "Didier Colin");
		assets().fonts.rpg.printC(graphics.back(), xcred, y7 + TITLE_GAP + 3 * LINE_GAP, "Sylvain Bugat");
		assets().fonts.rpg.printC(graphics.back(), xcred, y7 + TITLE_GAP + 4 * LINE_GAP, "Benjamin Karaban");

		int y8 = y7 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		assets().fonts.rpg.printC(graphics.back(), xcred, y8, "BETA TEST");
		assets().fonts.rpg.printC(graphics.back(), xcred, y8 + TITLE_GAP + 0 * LINE_GAP, "Jérémie Khodja");
		assets().fonts.rpg.printC(graphics.back(), xcred, y8 + TITLE_GAP + 1 * LINE_GAP, "Julien Areas");
		assets().fonts.rpg.printC(graphics.back(), xcred, y8 + TITLE_GAP + 2 * LINE_GAP, "Carlos Sédille");
		assets().fonts.rpg.printC(graphics.back(), xcred, y8 + TITLE_GAP + 3 * LINE_GAP, "Juliette Karaban");

		int y9 = y8 + 4 * LINE_GAP + SECTION_GAP + TITLE_GAP;
		int y10 = y9;
		int y11 = y10;

		if (theEnd) {
			assets().fonts.rpg.printC(graphics.back(), xcred, y9, "THANKS TO");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 0 * LINE_GAP, "Arnaud Dubois");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 1 * LINE_GAP, "Carole Schmitt");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 2 * LINE_GAP, "Aasterion staff");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 3 * LINE_GAP, "Aurore Anger");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 4 * LINE_GAP, "Cécile Baudoncourt");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 5 * LINE_GAP, "Nicolas Clément");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 6 * LINE_GAP, "Nathalie Morin");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 7 * LINE_GAP, "Florent and Thomas");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 8 * LINE_GAP, "Bruno Singer");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 9 * LINE_GAP, "Francois Deschamps");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 10 * LINE_GAP, "Colin Auger");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 11 * LINE_GAP, "TP122 students");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 12 * LINE_GAP, "Jeff");
			assets().fonts.rpg.printC(graphics.back(), xcred, y9 + TITLE_GAP + 13 * LINE_GAP, "Flavy");

			y10 = y9 + 14 * LINE_GAP + SECTION_GAP + TITLE_GAP;

			assets().fonts.rpg.printC(graphics.back(), xcred, y10, "SPECIAL THANKS TO");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 0 * LINE_GAP, "La Guinguette Pirate");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 1 * LINE_GAP, "TiPunch power!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 3 * LINE_GAP, "La Pirogue");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 4 * LINE_GAP, "I love you Séverine!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 6 * LINE_GAP, "Le Montbauron");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 7 * LINE_GAP, "mind the 8 ball!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 9 * LINE_GAP, "La Fleche d'Or");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 10 * LINE_GAP, "avoid the toilets!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 12 * LINE_GAP, "Hippochine Restaurant");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 13 * LINE_GAP, "great music!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 15 * LINE_GAP, "Vitigno Pizzeria");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 16 * LINE_GAP, "pizza! good!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 18 * LINE_GAP, "Max Linder Theatre");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 19 * LINE_GAP, "THX rules!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 21 * LINE_GAP, "UNEF");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 22 * LINE_GAP, "cheap beer 4 all!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 24 * LINE_GAP, "Hayao Myazaki");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 25 * LINE_GAP, "Totoro! To-to-ro!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 27 * LINE_GAP, "Kevin Smith");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 28 * LINE_GAP, "thirty seven?");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 30 * LINE_GAP, "Peter Jackson");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 31 * LINE_GAP, "Bad Taste rules!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 33 * LINE_GAP, "Monty Python");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 34 * LINE_GAP, "behind the rabbit?");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 36 * LINE_GAP, "John Boorman");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 37 * LINE_GAP, "Zardoz has spoken!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 39 * LINE_GAP, "Luc Besson");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 40 * LINE_GAP, "nah... just kidding!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 42 * LINE_GAP, "Christopher Lambert");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 43 * LINE_GAP, "watch your back!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 45 * LINE_GAP, "Jeremy Irons");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 46 * LINE_GAP, "lord Profion!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 48 * LINE_GAP, "Wayne Scott");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 49 * LINE_GAP, "Rambo! Rambo!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 51 * LINE_GAP, "Les Joyeux Urbains");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 52 * LINE_GAP, "va chez ta mère!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 54 * LINE_GAP, "Toss");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 55 * LINE_GAP, "da! da! da!");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 57 * LINE_GAP, "Le cassoulet");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 58 * LINE_GAP, "best served at 4 AM");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 60 * LINE_GAP, "Gamedev.net");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 61 * LINE_GAP, "DirectX vs hamsters");

			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 64 * LINE_GAP, "THANK");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 65 * LINE_GAP, "YOU");
			assets().fonts.rpg.printC(graphics.back(), xcred, y10 + TITLE_GAP + 66 * LINE_GAP, "FOR PLAYING");

			y11 = y10 + 72 * LINE_GAP + SECTION_GAP + TITLE_GAP;
		}

		assets().fonts.rpg.printC(graphics.back(), xcred, y11, "LOADED Studio");
		assets().fonts.rpg.printC(graphics.back(), xcred, y11 + LINE_GAP, "May 2002");

		last_y = y11 + 2 * LINE_GAP;

		if (t.elapsed() >= 12500 && theEnd) {
			npage++;
			npage %= pbk_cred.anim("pages").size();
			t.Reset();
		}

		graphics.Flip();
		co_await nextFrame();
	}
}
