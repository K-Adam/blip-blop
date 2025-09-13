#define GAME_CPP_FILE

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
#include "core/config.h"
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
#include "core/assets.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "entities/make_bonus.h"
#include "menus/menu_game.h"
#include "menus/title_screen.h"
#include "level/weather_snow.h"
#include "level/weather_rain.h"
#include "level/scroll.h"
#include "entities/cool_text.h"
#include "entities/shots/shot_bb.h"
#include "entities/shots/shot_bb_cow.h"
#include "core/text_data.h"
#include "entities/vehicle.h"
#include <json.h>
#include "core/asset_path.h"
#include <base64.h>
#include "engine/half_tone.h"

using json = nlohmann::json;

Character dummyPlayer;

#define TXT_TIME 828

Game::Game() : next_raindrop(0), next_snowflake(0), show_fps(false), show_lists(false) { briefing = false; }

// Out of line so the owned entity types are complete here
Game::~Game() = default;

Task<> Game::playGame(int player_count, int player_idx) {
	if (app_killed) co_return;

	CINEPlayer cine;
	bool letsgo = true;
	int i;

	mbk_interl.stop();

	if (!loadGame()) {
		releaseGame();
		co_return;
	}

	wait_for_death = 0;
	current_track = -1;

	if (player_idx == 0)
		player1 = std::make_unique<Blip>();
	else
		player1 = std::make_unique<Blop>();

	player1->ctrl = &controllers_[0];
	list_players.push_back(player1.get());

	if (player_count == 2) {
		if (player_idx == 0)
			player2 = std::make_unique<Blop>();
		else
			player2 = std::make_unique<Blip>();

		player2->ctrl = &controllers_[1];
		list_players.push_back(player2.get());
	}

	cowBombOn = false;
	last_perfect1 = last_perfect2 = false;
	int nbLevels = 0;
	i = 0;

	while (i < nb_part && letsgo && !app_killed) {
		if (type_part[i] == PART_LEVEL) {
			letsgo = co_await playLevel(file_names[i].c_str(), type_lvl[i]);

			if (!cowBombOn) {
				if (player1 != NULL) player1->nb_cow_bomb = 1;

				if (player2 != NULL) player2->nb_cow_bomb = 1;

				cowBombOn = true;
			}
		} else if (type_part[i] == PART_BRIEFING) {
			showBriefing(file_names[i].c_str());
		} else {
			co_await cine.playScene(file_names[i].c_str(), backSurface);
		}

		i += 1;
		co_await nextFrame();
	}

	if (letsgo && !app_killed) {
		MusicBank mbk;
		MusicBank mbk2;
		PictureBank pbk;

		pbk.loadGFX("theend.gfx");
		mbk.open("credits.mbk");
		mbk2.open("end.mbk", false);
		cine.loadPBK("end.gfx");
		mbk.play(1);
		co_await cine.playScene("end.cin", backSurface);
		mbk.stop();

		pbk[1]->PasteTo(backSurface, 0, 0);
		graphics.Flip();
		pbk[1]->PasteTo(backSurface, 0, 0);
		graphics.Flip();

		if (!in.anyKeyPressed()) mbk2.play(0);

		co_await in.waitClean();
		co_await in.waitKey();
		co_await in.waitClean();
		mbk2.stop();

		mbk.play(0);
		co_await showCredits(true);

		pbk[0]->PasteTo(backSurface, 0, 0);
		graphics.Flip();
		pbk[0]->PasteTo(backSurface, 0, 0);
		graphics.Flip();

		co_await in.waitClean();
		co_await in.waitKey();
		co_await in.waitClean();

		mbk.stop();

		co_await showPE(false);
	}

	releaseGame();

	pbk_inter.loadGFX("inter.gfx");

	if (!skipped && !app_killed) {
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
	int p1_life;  // Restored after a bonus level
	bool p1_bringBack;
	int p2_life;
	bool p2_bringBack;

	players_dead = false;
	level_done = false;
	skipped = false;
	wait_for_victory = -1;

	timer_tick = 0;
	go_.Reset();

	scroll_locked = false;
	scroll_speed = 0;
	no_scroll1 = false;
	no_scroll2 = false;

	hold_fire = false;

	weather_intensity = 0;

	n_cache = 0;

	for (int i = 0; i <= 10; i++) game_flag[i] = 0;

	clearCoolText();

	if (!loadLevel(level_name)) co_return false;

	if (std::string_view(level_name) == "lvl/snorkniv.lvl" || std::string_view(level_name) == "lvl/snorkniv2.lvl") {
		okFlamethrower = false;
	} else {
		okFlamethrower = true;
	}

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
	} else {
		p1_bringBack = false;
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
	} else {
		p2_bringBack = false;
	}

	if (type == LVL_BONUS || type == LVL_COMPLETE || type == LVL_FIRST || type == LVL_END) {
		nb_enemies_created = 0;

		if (player1 != NULL) {
			player1->setKilled(0);

			if (type != LVL_END) {
				player1->status = STATE_COME_BACK;
				player1->y = -50;
				player1->y_to_go = ystart1;
			}

			player1->weapon_id = ID_M16;
			player1->weapon_anim_delay = 3;
			player1->nb_weapon_steps = 5;
			player1->fire_interval = 10;
			player1->gun_turn_delay = 1;
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

			player2->weapon_id = ID_M16;
			player2->weapon_anim_delay = 3;
			player2->nb_weapon_steps = 5;
			player2->fire_interval = 10;
			player2->gun_turn_delay = 1;
			player2->perfect = true;
			player2->hp = 5;

			if (last_perfect2) {
				player2->setSuperWeapon();
			}
		}
	}

	last_perfect1 = last_perfect2 = false;

	bonus_level = type == LVL_BONUS;

	Sprite* s = list_players[0];
	offset = s->x;
	offset -= (offset % 640);

	n_img = 0;
	xTex = 0;
	n_cache = 0;
	next_x = 0;

	for (Chrono pause; pause.elapsed() < 2000 && !app_killed;) {
		graphics.Flip();
		co_await nextFrame();
	}

	if (briefing) {
		Rect r;

		r.left = 0;
		r.right = 640;
		r.top = 0;
		r.bottom = 480;

		backSurface->FillRect(&r, 0);

		pbk_briefing[0]->PasteTo(backSurface, 0, 0);
		fnt_rpg.printC(backSurface, 320, 460, "Press a key to start.");
		graphics.Flip();

		mbk_inter.play(2);
		co_await in.waitClean();
		co_await in.waitKey();
		briefing = false;
	}

	if (!app_killed) debug << "Now getting into game loop.\n";

	updateEvents();
	updateAll();

	update_regulator_.Skip();

	while (!players_dead && !level_done && !skipped && !app_killed) {
		co_await gameLoop();
		updateVictoryAndDefeat();

		co_await nextFrame();
	}

	if (players_dead) debug << "Players died\n";
	if (level_done) debug << "Level complete\n";
	if (skipped) debug << "User skipped\n";
	if (app_killed) debug << "Application killed\n";

	if (player1 != NULL) {
		player1->endLevel();
	}

	if (player2 != NULL) {
		player2->endLevel();
	}

	current_track = -1;
	if (music_on) mbk_level.stop();

	// A bonus level gives the lives back and cannot be lost
	if (type == LVL_BONUS && !skipped && !app_killed) {
		if (player1 != NULL && p1_bringBack) {
			if (player1->destroyed && p1_life > 0) {
				player1->destroyed = false;
				list_players.push_back(player1.get());
			}

			player1->nb_life = p1_life;
		}

		if (player2 != NULL && p2_bringBack) {
			if (player2->destroyed && p2_life > 0) {
				player2->destroyed = false;
				list_players.push_back(player2.get());
			}

			player2->nb_life = p2_life;
		}

		level_done = true;
	}

	if (level_done && !players_dead && type != LVL_END) {
		drawAll(false);
		graphics.Flip();
		drawAll(false);

		if ((type == LVL_BONUS && !skipped && game_flag[FLAG_TIMER] > 0) || type == LVL_COMPLETE || type == LVL_LAST) {
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

	bool loaded = load("gfx_decors", "gfx/", "background", true, [](const char* f) { return pbk_decor.loadGFX(f); }) &&
				  load("gfx_niveau", "gfx/", "level stuff", false, [](const char* f) { return pbk_level.loadGFX(f); }) &&
				  load("gfx_ennemis", "gfx/", "ennemies", false, [](const char* f) { return pbk_enemies.loadGFX(f); }) &&
				  load("sbk_ennemis", "sfx/", "SBK", false, [](const char* f) { return sbk_level.loadSFX(f); }) &&
				  load("fichier_mbk", "mbk/", "MBK", false,
					   [](const char* f) {
						   current_mbk = f;
						   return !music_on || mbk_level.open(f);
					   }) &&
				  load("fichier_rpg", "rpg/", "RPG file", false,
					   [&](const char* f) {
						   rpg.attachFile(f);
						   return true;
					   }) &&
				  load("gfx_rpg", "gfx/", "RPG GFX", false, [](const char* f) { return pbk_rpg.loadGFX(f); });
	if (!loaded) return false;
	if (data["fichier_mbk"].get<std::string>().empty()) current_mbk.clear();

	scr_level_size = data["num_decor"].size();
	level_size = scr_level_size * 640;

	auto platform_str = from_base64(data["y_plat"]);
	auto solid_walls_str = from_base64(data["murs_opaques"]);
	auto bloody_walls_str = from_base64(data["murs_sanglants"]);
	size_t level_size_8 = level_size / 8;
	if (platform_str.size() < NB_MAX_PLATFORMS * level_size * sizeof(int32_t) || solid_walls_str.size() < 60 * level_size_8 ||
		bloody_walls_str.size() < 60 * level_size_8) {
		debug << "Game::chargeNiveau() -> Truncated collision data in <" << json_path << ">\n";
		return false;
	}

	num_decor.resize(scr_level_size);
	for (size_t i = 0; i < scr_level_size; i++) {
		num_decor[i] = data["num_decor"][i];
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

	y_platform.assign(NB_MAX_PLATFORMS, std::vector<int32_t>(level_size));
	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		std::memcpy(y_platform[i].data(), &platform_str[i * level_size * sizeof(int32_t)], level_size * sizeof(int32_t));
	}

	solid_walls.resize(60);
	for (size_t i = 0; i < 60; i++) {
		solid_walls[i].assign(solid_walls_str.begin() + i * level_size_8, solid_walls_str.begin() + (i + 1) * level_size_8);
	}

	bloody_walls.resize(60);
	for (size_t i = 0; i < 60; i++) {
		bloody_walls[i].assign(bloody_walls_str.begin() + i * level_size_8, bloody_walls_str.begin() + (i + 1) * level_size_8);
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
				event_midground->id = event["id"];
				event_midground->x = event["x"];
				event_midground->y = event["y"];

				list_event_dormant.push_back(std::move(event_midground));
				break;
			}

			case EVENTID_FOREGROUND: {
				auto event_foreground = std::make_unique<EventForeground>();

				event_foreground->bg_id = event["id_fond"];
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
		}
	}

	debug << "Successfully loaded all level files\n";

	return true;
}

void Game::releaseLevel() {
	num_decor.clear();
	y_platform.clear();
	bloody_walls.clear();
	solid_walls.clear();

	list_event_dormant.clear();
	list_event.clear();

	list_bb_shots.clear();
	list_cow.clear();
	list_impacts.clear();

	list_vehicles.clear();

	list_enemies.clear();
	list_enemy_shots.clear();
	list_enemy_gens.clear();

	list_bonus.clear();
	list_gen_bonus.clear();

	list_txt_cool.clear();

	list_animated_bgs.clear();
	list_static_bgs.clear();
	list_foregrounds.clear();
	list_moving_platforms.clear();

	list_splatters.clear();
	list_gore.clear();

	list_weather.clear();
	list_bubbles.clear();
}

void Game::updateAll() {
	phase = !phase;

	if (phase) slow_phase = !slow_phase;

	okBonus = false;

	if (player1 != NULL) {
		okBonus = okBonus || player1->okBonus();
	}

	if (player2 != NULL) {
		okBonus = okBonus || player2->okBonus();
	}

	if (!no_scroll1 && !no_scroll2) updateScrolling(false);

	updateEvents();

	// Scroll again so locks set by the events apply
	if (!no_scroll1 && !no_scroll2) updateScrolling();

	updateCheat();

	if (game_flag[FLAG_BUBBLES]) updateBubbles();

	if (weather_type == WEATHER_RAIN || weather_type == WEATHER_SNOW) updateWeather();

	UpdateCollection(list_static_bgs);
	UpdateCollection(list_animated_bgs);
	UpdateCollection(list_moving_platforms);
	UpdateCollection(list_vehicles);
	UpdateCollection(list_players);
	UpdateCollection(list_bb_shots);
	UpdateCollection(list_cow);
	UpdateCollection(list_impacts);
	UpdateCollection(list_enemies);
	UpdateCollection(list_gore);
	UpdateCollection(list_enemy_shots);
	UpdateCollection(list_splatters);
	UpdateCollection(list_bonus);
	UpdateCollection(list_foregrounds);

	UpdateCollection(list_txt_cool);
	UpdateCollection(list_enemy_gens);
	UpdateCollection(list_gen_bonus);
	updateLock();
	updateHoldFire();
	updateTargetedPlayer();
	updateFlags();
	updateGoArrow();

	if (weather_type == WEATHER_DISTORT && weather_intensity != 0) updateDeformation();

	manageCollisions();

	cleanLists();
}

void Game::drawAll(bool flip) {
	if (!flip) {
		phase = false;
		slow_phase = false;
	}

	fps_current_count += 1;
	static Uint32 fps_start = SDL_GetTicks();
	if (SDL_GetTicks() - fps_start >= 1000) {
		fps_count = fps_current_count;
		fps_current_count = 0;
		fps_start = SDL_GetTicks();
	}

	drawScrolling();
	DrawCollection(list_static_bgs);
	DrawCollection(list_animated_bgs);

	if (game_flag[FLAG_BUBBLES]) {
		DrawCollection(list_bubbles);
	}

	DrawCollection(list_moving_platforms);
	DrawCollection(list_impacts);
	DrawCollection(list_gore);

	DrawCollection(list_enemies);
	DrawCollection(list_enemy_shots);
	DrawCollection(list_bonus);
	DrawCollection(list_bb_shots);
	DrawCollection(list_players);
	DrawCollection(list_vehicles);
	DrawCollection(list_impacts);
	DrawCollection(list_cow);
	DrawCollection(list_weather);
	DrawCollection(list_foregrounds);

	if (weather_type == WEATHER_DISTORT && weather_intensity != 0) drawDeformation();

	drawHUB();
	drawTimer();
	go_.Draw();
	DrawCollection(list_txt_cool);

	drawDebugInfos();

	// Hides the glitchy strip at the right edge
	if (vbuffer_wide == 640) {
		Rect r;

		r.left = 638;
		r.right = 640;
		r.top = 0;
		r.bottom = 480;

		backSurface->FillRect(&r, 0);
	}

	Rect r;

	r.top = 0;
	r.left = 0;
	r.right = 640;
	r.bottom = 480;

	if (flip) {
		graphics.Flip();
	}
}

Task<> Game::gameLoop() {
	int n_updates = update_regulator_.Step();
	// A dialog starts as soon as an event asks for it, like it did inside updateAll
	for (; n_updates > 0 && rpg_to_play == -1; --n_updates) {
		updateAll();
	}

	co_await updateMenu();
	co_await updateRPG();

	drawAll();
}

void Game::releaseGame() { list_players.clear(); }

bool Game::loadGame() {
	debug << "---------------------------------------------------------------\n";
	debug << "Loading global game files...\n";
	debug << "---------------------------------------------------------------\n";

	if (!pbk_blip.loadGFX("blip.gfx")) return false;

	debug << "Successfully loaded <blip.gfx>\n";

	if (!pbk_blop.loadGFX("blop.gfx")) return false;

	debug << "Successfully loaded <blop.gfx>\n";

	if (!pbk_bb.loadGFX("bb.gfx")) return false;

	debug << "Successfully loaded <bb.gfx>\n";

	if (!pbk_misc.loadGFX("misc.gfx")) return false;

	debug << "Successfully loaded <misc.gfx>\n";

	if (!pbk_rpg_bb.loadGFX("rpg_bb.gfx")) return false;

	debug << "Successfully loaded <rpg_bb.gfx>\n";

	if (!fnt_score_blip.load("scorei.lft")) return false;

	debug << "Successfully loaded <scorei.lft>\n";

	if (!fnt_score_blop.load("scoreo.lft")) return false;

	debug << "Successfully loaded <scoreo.lft>\n";

	if (!fnt_ammo.load("ammo1.lft")) return false;

	debug << "Successfully loaded <ammo1.lft>\n";

	if (!fnt_ammo_used.load("ammo2.lft")) return false;

	debug << "Successfully loaded <ammo2.lft>\n";

	if (!sbk_misc.loadSFX("misc.sfx")) return false;

	debug << "Successfully loaded <misc.sfx>\n";

	if (!sbk_bb.loadSFX("bb.sfx")) return false;

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
	RemoveDestroyed(list_players);
	RemoveDestroyed(list_static_bgs);
	RemoveDestroyed(list_bb_shots);
	RemoveDestroyed(list_cow);
	RemoveDestroyed(list_bubbles);
	RemoveDestroyed(list_impacts);
	RemoveDestroyed(list_enemies);
	RemoveDestroyed(list_bonus);
	RemoveDestroyed(list_enemy_gens);
	RemoveDestroyed(list_gen_bonus);
	RemoveDestroyed(list_txt_cool);
	RemoveDestroyed(list_animated_bgs);
	RemoveDestroyed(list_foregrounds);
	RemoveDestroyed(list_splatters);
	RemoveDestroyed(list_enemy_shots);
	RemoveDestroyed(list_weather);
	RemoveDestroyed(list_gore);
	RemoveDestroyed(list_moving_platforms);
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
	for (Shot* shot : list_bb_shots) {
		for (auto& enemy : list_enemies) {
			if (shot->collision(enemy.get())) {
				enemy->takeHit(shot);
			}
		}
	}

	for (auto& shot : list_cow) {
		for (auto& enemy : list_enemies) {
			if (shot->collision(enemy.get())) {
				enemy->takeHit(shot.get());
			}
		}
	}

	for (auto& bonus : list_bonus) {
		for (Ball* ball : list_players) {
			if (bonus->collision(ball)) {
				bonus->onPickup(ball);
			}
		}
	}

	if (wait_for_victory <= 0) {
		for (auto& enemy : list_enemies) {
			for (Ball* hero : list_players) {
				if (enemy->collision(hero)) hero->takeHit(enemy->damage());
			}
		}

		for (auto& shot : list_enemy_shots) {
			for (Ball* hero : list_players) {
				if (shot->collision(hero)) {
					hero->takeHit(shot->damage());
				}
			}
		}
	}
}

void Game::updateLock() {
	if (!scroll_locked) return;

	if ((cond_end_lock == 0 && list_enemies.empty()) || (cond_end_lock == 1 && list_enemy_gens.empty()) ||
		(cond_end_lock == 2 && game_flag[flag_end_lock] == val_end_lock) || (cond_end_lock == 3 && game_flag[flag_end_lock] >= val_end_lock)) {
		scroll_locked = false;
		go_.Come();
	}
}

void Game::updateHoldFire() {
	if (!hold_fire) return;

	if (game_flag[flag_hold_fire] == val_hold_fire) hold_fire = false;
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

	for (int i = 0; i < hp; i++) pbk_bb[201 - i]->BlitTo(backSurface, x + x_hp[i], y + y_hp[i]);

	for (int i = hp; i < 5; i++) pbk_bb[196 - i]->BlitTo(backSurface, x + x_hp[i], y + y_hp[i]);
}

void Game::drawDebugInfos() {
	if (show_fps) {
		fnt_rpg.print(backSurface, 10, 130, ("FPS = " + std::to_string(fps_count)).c_str());
	}
}

void Game::updateTargetedPlayer() {
	static int targeted_idx = 0;

	if (list_players.empty()) {
		dummyPlayer.x = offset + 320;
		dummyPlayer.y = y_platform[0][offset + 320];
		targeted_player = &dummyPlayer;
		return;
	}

	targeted_idx += 1;
	targeted_idx %= list_players.size();
	targeted_player = list_players[targeted_idx];
}

Task<> Game::updateRPG() {
	if (rpg_to_play == -1) co_return;

	bool continued = true;

	rpg.startPlay(rpg_to_play);

	while (continued && !app_killed) {
		drawAll(false);
		continued = rpg.drawScene(backSurface);
		graphics.Flip();

		co_await nextFrame();
	}

	rpg.stopPlay();
	co_await in.waitClean();
	rpg_to_play = -1;
	update_regulator_.Skip();
}

void Game::updateVictoryAndDefeat() {
	if (list_players.empty()) {
		wait_for_death += 1;

		if (wait_for_death >= 200) players_dead = true;

		return;
	}

	if (offset >= vic_x && game_flag[vic_flag1] == vic_val1 && game_flag[vic_flag2] == vic_val2) {
		hold_fire = true;
		wait_for_victory += 1;

		if (game_flag[1] == 999) wait_for_victory = 200;

		if (wait_for_victory >= 200) level_done = true;
	} else {
		if (wait_for_victory > 0) wait_for_victory = 0;
	}
}

void Game::updateFlags() {
	game_flag[FLAG_NB_GEN] = (int)list_enemy_gens.size();
	game_flag[FLAG_NB_ENEMIES] = (int)list_enemies.size();
	makeb_current_mode = game_flag[FLAG_BONUS];

	if (game_flag[FLAG_TIMER] > 0 && wait_for_victory == -1) {
		timer_tick += 1;
		timer_tick %= 80;

		if (timer_tick == 0) {
			game_flag[FLAG_TIMER] -= 1;

			if (game_flag[FLAG_TIMER] == 0 && bonus_level) level_done = true;
		}
	}
}

void Game::drawTimer() {
	if (game_flag[FLAG_TIMER] > 0) {
		fnt_cool.printC(backSurface, 320, 20, txt_data[TXT_TIME]);
		fnt_cool.printC(backSurface, 320, 50, std::to_string(game_flag[FLAG_TIMER]));
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

		menu.Draw(backSurface);
		graphics.Flip();

		co_await in.waitClean();

		while (!(r = menu.Update()) && !app_killed) {
			drawAll(false);
			menu.Draw(backSurface);

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

	if (!cheat_on) return;

	if (player1 != NULL) {
		if (in.scanKey(DIK_F1)) player1->ammo += 10;

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
		if (in.scanKey(DIK_F5)) player2->ammo += 10;

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
		if (life_up > 0) fnt_rpg.printC(backSurface, xbase + 105, 270, "LIFE UP");

		auto line = [&](int y, const char* label, int value) {
			fnt_rpg.print(backSurface, xbase, y, label);
			fnt->printR(backSurface, xbase + 210, y, std::to_string(value).c_str());
		};
		if (perfect > 0) line(180, "PERFECT", c_perfect);
		if (obj > 0) line(200, "Objective", c_obj);
		line(220, "Casualties", c_killed);
		fnt_rpg.printR(backSurface, xbase + 220, 220, "%");
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

	systemSurface->BltFast(0, 0, backSurface, NULL);

	Rect r;
	r.top = 100;
	r.left = 60;
	r.right = 580;
	r.bottom = 400;
	halfTone(systemSurface, &r);

	mbk_inter.play(0);

	if (list_players.size() == 2) {
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
		t.fnt = t.player->id_ball == ID_BLIP ? &fnt_score_blip : &fnt_score_blop;
		if (nb_enemies_created > 0) t.killed = (100 * t.player->getKilled()) / nb_enemies_created;
		if (!bonus || (game_flag[FLAG_TIMER] > 0 && !players_dead)) t.obj = 50000;
		if (t.obj > 0 && t.player->perfect) t.perfect = 50000;
		t.total = t.player->getScore();
		if (bonus_failed) t.perfect = t.obj = t.killed = 0;
	}
	if (bonus_failed) game_flag[FLAG_TIMER] = 0;

	int tally_tick = 0;

	// Runs at least once so the screen shows even when no player is left to tally
	do {
		tally_tick += 1;
		tally_tick %= 3;

		if (in.anyKeyPressed()) tally_tick = 0;

		if (tally_tick == 0) {
			for (auto& t : totals) t.step(game_flag[FLAG_TIMER]);
		}

		backSurface->BltFast(0, 0, systemSurface, NULL);

		fnt_cool.printC(backSurface, 320, 120, bonus_failed ? "BONUS STAGE FAILED!" : "LEVEL COMPLETE!");

		for (auto& t : totals) {
			if (t.fnt) t.draw(game_flag[FLAG_TIMER]);
		}

		graphics.Flip();

		co_await nextFrame();
	} while (!app_killed && (totals[0].calc < 4 || totals[1].calc < 4));

	if (!app_killed) {
		co_await in.waitClean();
		co_await in.waitKey();
	}

	for (auto& t : totals) {
		if (t.fnt) t.player->setScore(t.total);
	}

	mbk_inter.stop();
}

void Game::updateWeather() {
	const static int raindrop_speeds[] = {3, 5, 7, 8, 9, 10};

	for (auto& pl : list_weather) {
		pl->update();
	}

	while (list_weather.size() < (size_t)weather_intensity) {
		if (weather_type == WEATHER_SNOW) {
			next_snowflake = (next_snowflake + 1) % NB_SNOWFLAKES;
			WeatherSnow* snowflake = &snow[next_snowflake];

			int d = rand() % 3;

			snowflake->destroyed = false;
			snowflake->xbase = offset + rand() % 800;
			snowflake->y = rand() % 480 - 500;

			if (d <= 1)
				snowflake->pic = pbk_misc[71];
			else
				snowflake->pic = pbk_misc[72];

			snowflake->dy = weather_intensity / 20 + d;
			snowflake->phi = rand() % 360;
			snowflake->xwide = 10 + d * 4;

			if (solid_wall(snowflake->xbase, 0)) snowflake->y -= 550;

			list_weather.emplace_back(snowflake);
		} else if (weather_type == WEATHER_RAIN) {
			next_raindrop = (next_raindrop + 1) % NB_RAINDROPS;
			WeatherRain* raindrop = &rain[next_raindrop];

			int d = rand() % 4;

			raindrop->destroyed = false;
			raindrop->x = offset + rand() % 800;
			raindrop->y = rand() % 480 - 480;
			raindrop->pic = pbk_misc[65 + d];
			raindrop->dy = 7 + d * 2;

			if (solid_wall(raindrop->x, 0)) raindrop->y -= 550;

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
	if (last_x_go_ != offset) {
		last_x_go_ = offset;
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

		backSurface->BltFast(x, y, backSurface, &r);

		if (xt < 0) {
			r.left = 640 + xt;
			r.right = 640;
		} else {
			r.left = 0;
			r.right = xt;
		}

		backSurface->FillRect(&r, 0);
	}
}

void Game::updateBubbles() {
	for (Ball* pl : list_players) {
		spawnBubble(pl);
	}

	for (auto& pl : list_enemies) {
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
	s->wait_bubble += rand() % 10;

	if (s->wait_bubble > 300) {
		Bubble* b = new Bubble();

		b->xbase = s->x;
		b->y = s->y - 10;
		b->pic = pbk_misc[78 + rand() % 3];
		b->dphi = 5 + rand() % 3;
		b->dy = -(2 + rand() % 2);

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

void Game::drawLoading() {
	Rect r;

	r.left = 0;
	r.right = 640;
	r.top = 195;
	r.bottom = 245;

	fnt_cool.printC(backSurface, 320, 205, "LOADING");
}

Task<> Game::getHiscore() {
	if (player1 != NULL && hi_scores.isGood(player1->getScore())) co_await getName(player1.get(), 1);

	if (player2 != NULL && hi_scores.isGood(player2->getScore())) co_await getName(player2.get(), 2);
}

Task<> Game::getName(Player* hero, int player_num) {
	const std::string title = "PLAYER " + std::to_string(player_num) + " GOT A HIGH SCORE!";
	std::string name;
	int key = -1;
	int x = 640;

	while (!app_killed && x >= 40) {
		x -= 20;

		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		fnt_menu.printC(backSurface, 320 - x, 160, title);
		fnt_menu.printC(backSurface, 320 + x, 210, "PLEASE ENTER YOUR NAME :");

		graphics.Flip();

		co_await nextFrame();
	}

	while (!app_killed && key != DIK_RETURN) {
		co_await in.waitClean();
		key = co_await in.waitKey();
		if (in.scanKey(DIK_RETURN)) key = DIK_RETURN;

		std::string key_tmp = DIK_to_string(key);

		if (key == DIK_SPACE && name.size() < 19) name += ' ';
		if (key == DIK_BACKSPACE && !name.empty()) {
			name.pop_back();
		} else if (key_tmp.size() == 1 && name.size() < 19) {
			name += key_tmp[0];
		}

		pbk_inter[1]->PasteTo(backSurface, 0, 0);
		fnt_menu.printC(backSurface, 320 - x, 160, title);
		fnt_menu.printC(backSurface, 320 + x, 210, "PLEASE ENTER YOUR NAME :");
		fnt_menu.printC(backSurface, 320, 260, name);
		graphics.Flip();

		co_await nextFrame();
	}

	while (!app_killed && x < 640) {
		x += 20;

		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		fnt_menu.printC(backSurface, 320 - x, 160, title);
		fnt_menu.printC(backSurface, 320 + x, 210, "PLEASE ENTER YOUR NAME :");
		fnt_menu.printC(backSurface, 320, 260 + x, name);

		graphics.Flip();

		co_await nextFrame();
	}

	hi_scores.add(hero->getScore(), name);
	save_settings();
}

Task<> Game::showGameOver() {
	Chrono t;

	mbk_inter.play(1);

	while (!app_killed && t.elapsed() <= 5000 && !in.anyKeyPressed()) {
		pbk_inter[0]->PasteTo(backSurface, 0, 0);
		graphics.Flip();

		co_await nextFrame();
	}
	mbk_inter.stop();
}

Task<> Game::showHighScores() {
	int x[HS_NB_SCORES];

	for (int i = 0; i < HS_NB_SCORES; i++) x[i] = 400 + 160 * i;

	Chrono scores_timer;
	while (!app_killed && !in.anyKeyPressed() && (scores_timer.elapsed() <= 10000)) {
		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		for (int i = 0; i < HS_NB_SCORES; i++) {
			if (x[i] > 0) x[i] -= 20;

			fnt_cool.printR(backSurface, 300 - x[i], 160 + 30 * i, std::to_string(hi_scores.getScore(i)).c_str());

			fnt_cool.print(backSurface, 320 + x[i], 160 + 30 * i, hi_scores.getName(i));
		}

		graphics.Flip();
		co_await nextFrame();
	}

	while (!app_killed && x[HS_NB_SCORES - 1] < 350) {
		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		for (int i = 0; i < HS_NB_SCORES; i++) {
			if (i == 0 || x[i - 1] >= 160) x[i] += 20;

			fnt_cool.printR(backSurface, 300 - x[i], 160 + 30 * i, std::to_string(hi_scores.getScore(i)).c_str());

			fnt_cool.print(backSurface, 320 + x[i], 160 + 30 * i, hi_scores.getName(i));
		}

		graphics.Flip();
		co_await nextFrame();
	}

	co_await in.waitClean();
}

Task<> Game::go() {
	TitleScreen menu;
	int r;
	static int attract_step = 0;

	{
		backSurface->FillRect(nullptr, 0);
		fnt_cool.printC(backSurface, 320, 240, "PLEASE WAIT");
	}

	CINEPlayer cine;

	cine.loadPBK("intro.gfx");
	mbk_interl.play(0);
	co_await cine.playScene("intro.cin", backSurface, true);

	menu.start();
	debug << "Title menu\n";
	co_await in.waitClean();

	Chrono diff_start;

	do {
		r = RET_CONTINUE;

		while (!r && !app_killed) {
			if (in.anyKeyPressed()) {
				diff_start.Reset();
			}

			if (diff_start.elapsed() >= 10000) {
				switch (attract_step) {
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
						co_await cine.playScene("intro.cin", backSurface, true);
						break;
				}

				attract_step = (attract_step + 1) % 8;

				menu.stop();
				menu.start();

				if (in.anyKeyPressed()) {
					diff_start.Reset();
					co_await in.waitClean();
				}
			} else {
				pbk_inter[1]->PasteTo(backSurface, 0, 0);

				r = menu.update();
				menu.draw(backSurface);

				graphics.Flip();
			}

			co_await nextFrame();
		}

		if (r == RET_START_GAME) {
			JoinScreen join;

			auto joined = JoinScreen::Output::Continue;
			while (joined == JoinScreen::Output::Continue && !app_killed) {
				joined = join.update();
				join.draw();
				graphics.Flip();

				co_await nextFrame();
			}

			if (joined == JoinScreen::Output::Start) {
				for (int i = 0; i < join.players(); i++) controllers_[i] = join.player(i);

				CharacterSelection select;

				auto selected = CharacterSelection::Output::Continue;
				while (selected == CharacterSelection::Output::Continue && !app_killed) {
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
				mbk_interl.play(0);
			}

			menu.stop();
			menu.start();
			diff_start.Reset();
		}

		co_await nextFrame();

	} while (r != RET_EXIT && !app_killed);

	menu.stop();

	mbk_interl.stop();
}

void Game::showBriefing(const char* fn) {
	pbk_briefing.loadGFX(fn);

	briefing = true;

	Rect r;

	r.left = 0;
	r.right = 640;
	r.top = 0;
	r.bottom = 480;

	backSurface->FillRect(&r, 0);

	pbk_briefing[0]->PasteTo(backSurface, 0, 0);
	fnt_rpg.printC(backSurface, 320, 460, "Loading...");
	graphics.Flip();
}

Task<> Game::showMainScreen() {
	Chrono t;

	while (!in.anyKeyPressed() && !app_killed && t.elapsed() <= 5000) {
		pbk_inter[1]->PasteTo(backSurface, 0, 0);
		graphics.Flip();

		co_await nextFrame();
	}
}

Task<> Game::showCredits(bool theEnd) {
	static const int NB_PAGE = 6;
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

	while (!app_killed && (!in.anyKeyPressed() || theEnd) && last_y > 0) {
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

			backSurface->FillRect(&r, 0);
		}

		if (theEnd) {
			pbk_cred[npage]->PasteTo(backSurface, 0, 0);
		} else {
			pbk_inter[1]->BlitTo(backSurface, 0, 0);
		}

		fnt_rpg.printC(backSurface, xcred, y + LINE_GAP, "Credits");

		int y1 = y + LINE_GAP + SECTION_GAP;

		fnt_rpg.printC(backSurface, xcred, y1, "CODE");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 0, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 1, "Sylvain Bugat");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 3, "2019 REVAMP");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 4, "Guillaume Sanchez");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 6, "Web port");
		fnt_rpg.printC(backSurface, xcred, y1 + TITLE_GAP + LINE_GAP * 7, "Adam Kecskes");

		int y2 = y1 + 4 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y2, "MUSIC AND SOUND");
		fnt_rpg.printC(backSurface, xcred, y2 + LINE_GAP, "DESIGN");
		fnt_rpg.printC(backSurface, xcred, y2 + TITLE_GAP + LINE_GAP, "Gerard Kelly");

		int y3 = y2 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y3, "ADDITIONAL MUSIC");
		fnt_rpg.printC(backSurface, xcred, y3 + TITLE_GAP + 0 * LINE_GAP, "El Mobo");
		fnt_rpg.printC(backSurface, xcred, y3 + TITLE_GAP + 1 * LINE_GAP, "Rik Ede");

		int y4 = y3 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y4, "ARTWORK");
		fnt_rpg.printC(backSurface, xcred, y4 + TITLE_GAP + LINE_GAP * 0, "Laurent Schneider");
		fnt_rpg.printC(backSurface, xcred, y4 + TITLE_GAP + LINE_GAP * 1, "Jérémie Comarmond");
		fnt_rpg.printC(backSurface, xcred, y4 + TITLE_GAP + LINE_GAP * 2, "Jérôme Karaban");
		fnt_rpg.printC(backSurface, xcred, y4 + TITLE_GAP + LINE_GAP * 3, "Didier Colin");
		fnt_rpg.printC(backSurface, xcred, y4 + TITLE_GAP + LINE_GAP * 4, "Sylvain Bugat");

		int y5 = y4 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y5, "ADDITIONAL SOUND");
		fnt_rpg.printC(backSurface, xcred, y5 + LINE_GAP, "AND VOCAL EFFECTS");
		fnt_rpg.printC(backSurface, xcred, y5 + TITLE_GAP + 1 * LINE_GAP, "Eric Khodja");
		fnt_rpg.printC(backSurface, xcred, y5 + TITLE_GAP + 2 * LINE_GAP, "Jérôme Karaban");
		fnt_rpg.printC(backSurface, xcred, y5 + TITLE_GAP + 3 * LINE_GAP, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y5 + TITLE_GAP + 4 * LINE_GAP, "Vincent Lagarrigue");

		int y6 = y5 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y6, "STORYLINE");
		fnt_rpg.printC(backSurface, xcred, y6 + TITLE_GAP + 0 * LINE_GAP, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y6 + TITLE_GAP + 1 * LINE_GAP, "Eric Khodja");

		int y7 = y6 + 2 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y7, "ORIGINAL CONCEPT");
		fnt_rpg.printC(backSurface, xcred, y7 + TITLE_GAP + 0 * LINE_GAP, "Laurent Schneider");
		fnt_rpg.printC(backSurface, xcred, y7 + TITLE_GAP + 1 * LINE_GAP, "Jérémie Comarmond");
		fnt_rpg.printC(backSurface, xcred, y7 + TITLE_GAP + 2 * LINE_GAP, "Didier Colin");
		fnt_rpg.printC(backSurface, xcred, y7 + TITLE_GAP + 3 * LINE_GAP, "Sylvain Bugat");
		fnt_rpg.printC(backSurface, xcred, y7 + TITLE_GAP + 4 * LINE_GAP, "Benjamin Karaban");

		int y8 = y7 + 5 * LINE_GAP + SECTION_GAP + TITLE_GAP;

		fnt_rpg.printC(backSurface, xcred, y8, "BETA TEST");
		fnt_rpg.printC(backSurface, xcred, y8 + TITLE_GAP + 0 * LINE_GAP, "Jérémie Khodja");
		fnt_rpg.printC(backSurface, xcred, y8 + TITLE_GAP + 1 * LINE_GAP, "Julien Areas");
		fnt_rpg.printC(backSurface, xcred, y8 + TITLE_GAP + 2 * LINE_GAP, "Carlos Sédille");
		fnt_rpg.printC(backSurface, xcred, y8 + TITLE_GAP + 3 * LINE_GAP, "Juliette Karaban");

		int y9 = y8 + 4 * LINE_GAP + SECTION_GAP + TITLE_GAP;
		int y10 = y9;
		int y11 = y10;

		if (theEnd) {
			fnt_rpg.printC(backSurface, xcred, y9, "THANKS TO");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 0 * LINE_GAP, "Arnaud Dubois");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 1 * LINE_GAP, "Carole Schmitt");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 2 * LINE_GAP, "Aasterion staff");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 3 * LINE_GAP, "Aurore Anger");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 4 * LINE_GAP, "Cécile Baudoncourt");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 5 * LINE_GAP, "Nicolas Clément");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 6 * LINE_GAP, "Nathalie Morin");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 7 * LINE_GAP, "Florent and Thomas");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 8 * LINE_GAP, "Bruno Singer");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 9 * LINE_GAP, "Francois Deschamps");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 10 * LINE_GAP, "Colin Auger");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 11 * LINE_GAP, "TP122 students");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 12 * LINE_GAP, "Jeff");
			fnt_rpg.printC(backSurface, xcred, y9 + TITLE_GAP + 13 * LINE_GAP, "Flavy");

			y10 = y9 + 14 * LINE_GAP + SECTION_GAP + TITLE_GAP;

			fnt_rpg.printC(backSurface, xcred, y10, "SPECIAL THANKS TO");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 0 * LINE_GAP, "La Guinguette Pirate");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 1 * LINE_GAP, "TiPunch power!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 3 * LINE_GAP, "La Pirogue");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 4 * LINE_GAP, "I love you Séverine!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 6 * LINE_GAP, "Le Montbauron");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 7 * LINE_GAP, "mind the 8 ball!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 9 * LINE_GAP, "La Fleche d'Or");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 10 * LINE_GAP, "avoid the toilets!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 12 * LINE_GAP, "Hippochine Restaurant");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 13 * LINE_GAP, "great music!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 15 * LINE_GAP, "Vitigno Pizzeria");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 16 * LINE_GAP, "pizza! good!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 18 * LINE_GAP, "Max Linder Theatre");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 19 * LINE_GAP, "THX rules!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 21 * LINE_GAP, "UNEF");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 22 * LINE_GAP, "cheap beer 4 all!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 24 * LINE_GAP, "Hayao Myazaki");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 25 * LINE_GAP, "Totoro! To-to-ro!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 27 * LINE_GAP, "Kevin Smith");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 28 * LINE_GAP, "thirty seven?");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 30 * LINE_GAP, "Peter Jackson");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 31 * LINE_GAP, "Bad Taste rules!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 33 * LINE_GAP, "Monty Python");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 34 * LINE_GAP, "behind the rabbit?");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 36 * LINE_GAP, "John Boorman");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 37 * LINE_GAP, "Zardoz has spoken!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 39 * LINE_GAP, "Luc Besson");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 40 * LINE_GAP, "nah... just kidding!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 42 * LINE_GAP, "Christopher Lambert");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 43 * LINE_GAP, "watch your back!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 45 * LINE_GAP, "Jeremy Irons");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 46 * LINE_GAP, "lord Profion!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 48 * LINE_GAP, "Wayne Scott");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 49 * LINE_GAP, "Rambo! Rambo!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 51 * LINE_GAP, "Les Joyeux Urbains");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 52 * LINE_GAP, "va chez ta mère!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 54 * LINE_GAP, "Toss");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 55 * LINE_GAP, "da! da! da!");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 57 * LINE_GAP, "Le cassoulet");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 58 * LINE_GAP, "best served at 4 AM");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 60 * LINE_GAP, "Gamedev.net");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 61 * LINE_GAP, "DirectX vs hamsters");

			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 64 * LINE_GAP, "THANK");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 65 * LINE_GAP, "YOU");
			fnt_rpg.printC(backSurface, xcred, y10 + TITLE_GAP + 66 * LINE_GAP, "FOR PLAYING");

			y11 = y10 + 72 * LINE_GAP + SECTION_GAP + TITLE_GAP;
		}

		fnt_rpg.printC(backSurface, xcred, y11, "LOADED Studio");
		fnt_rpg.printC(backSurface, xcred, y11 + LINE_GAP, "May 2002");

		last_y = y11 + 2 * LINE_GAP;

		if (t.elapsed() >= 12500 && theEnd) {
			npage++;
			npage %= pbk_cred.getSize();
			t.Reset();
		}

		graphics.Flip();
		co_await nextFrame();
	}
}
