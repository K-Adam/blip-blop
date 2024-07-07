#define GAME_CPP_FILE

#include <algorithm>

#include <fcntl.h>
#include <cstring>
#include <fstream>

#include "core/debug.h"
#include "core/misc.h"
#include "entities/blip.h"
#include "entities/blop.h"
#include "entities/bonus.h"
#include "entities/bubble.h"
#include "menus/character_selection.h"
#include "cinematics/cine_player.h"
#include "core/config.h"
#include "engine/dd_gfx.h"
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
#include "core/globals.h"
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

Game::Game() : next_goutte(0), next_flocon(0), show_fps(false), show_lists(false) { briefing = false; }

void Game::jouePartie(int nbj, int idj) {
	if (app_killed) return;

	CINEPlayer cine;
	bool letsgo = true;
	int i;

	// Bidon
	mbk_interl.stop();

	// Charge Tout
	if (!chargePartie()) {
		releasePartie();
		return;
	}

	wait_for_death = 0;
	current_zik = -1;

	if (idj == 0)
		player1 = std::make_unique<Blip>();
	else
		player1 = std::make_unique<Blop>();

	player1->ctrl = &ctrlP1;
	list_joueurs.push_back(player1.get());

	if (nbj == 2) {
		if (idj == 0)
			player2 = std::make_unique<Blop>();
		else
			player2 = std::make_unique<Blip>();

		player2->ctrl = &ctrlP2;
		list_joueurs.push_back(player2.get());
	}

	// Joue à tous les niveaux
	cowBombOn = false;
	last_perfect1 = last_perfect2 = false;
	int nbNiv = 0;
	i = 0;

	while (i < nb_part && letsgo && !app_killed) {
		if (type_part[i] == PART_LEVEL) {
			letsgo = joueNiveau(fic_names[i].c_str(), type_lvl[i]);

			if (!cowBombOn) {
				if (player1 != NULL) player1->nb_cow_bomb = 1;

				if (player2 != NULL) player2->nb_cow_bomb = 1;

				cowBombOn = true;
			}
		} else if (type_part[i] == PART_BRIEFING) {
			showBriefing(fic_names[i].c_str());
		} else {
			cine.playScene(fic_names[i].c_str(), primSurface, backSurface);
		}

		i += 1;
		yieldToBrowser();
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
		cine.playScene("end.cin", primSurface, backSurface);
		mbk.stop();

		pbk[1]->PasteTo(backSurface, 0, 0);
		DDFlip();
		pbk[1]->PasteTo(backSurface, 0, 0);
		DDFlip();

		if (!in.anyKeyPressed()) mbk2.play(0);

		in.waitClean();
		in.waitKey();
		in.waitClean();
		mbk2.stop();

		mbk.play(0);
		showCredits(true);

		pbk[0]->PasteTo(backSurface, 0, 0);
		DDFlip();
		pbk[0]->PasteTo(backSurface, 0, 0);
		DDFlip();

		in.waitClean();
		in.waitKey();
		in.waitClean();

		mbk.stop();

		showPE(false);
	}

	// Désalloue tout
	releasePartie();

	pbk_inter.loadGFX("inter.gfx");

	if (!skipped && !app_killed) {
		if (!letsgo) {
			showGameOver();
		}

		getHiscore();
		showHighScores();
	}

	player1.reset();
	player2.reset();
}

bool Game::joueNiveau(const char* nom_niveau, int type) {
	int p1_life;  // Si il s'agit d'un niveau bonus on
	bool p1_bringBack;
	int p2_life;  // conserve le nombre de vies des joueurs
	bool p2_bringBack;

	// Little init
	joueurs_morts = false;
	niveau_fini = false;
	skipped = false;
	wait_for_victory = -1;

	etape_timer = 0;
	go_.Reset();

	scroll_locked = false;
	scroll_speed = 0;
	no_scroll1 = false;
	no_scroll2 = false;

	hold_fire = false;

	intensite_meteo = 0;
	dy_tremblement = 0;
	etape_tremblement = 0;
	amplitude_tremblement = 0;

	n_cache = 0;

	for (int i = 0; i <= 10; i++) game_flag[i] = 0;

	clearTexteCool();

	if (!chargeNiveau(nom_niveau)) return false;

	if (std::string_view(nom_niveau) == "lvl/snorkniv.lvl" || std::string_view(nom_niveau) == "lvl/snorkniv2.lvl") {
		okLanceFlame = false;
	} else {
		okLanceFlame = true;
	}

	// Place les joueurs et initialise qq trucs
	if (player1 != NULL && player1->nb_life > 0) {
		player1->x = xstart1;
		player1->y = ystart1;
		player1->etat = ETAT_NORMAL;
		player1->etape = 0;
		player1->ss_etape = 0;
		player1->dir = BBDIR_DROITE;
		player1->tire = false;

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
		player2->etat = ETAT_NORMAL;
		player2->etape = 0;
		player2->ss_etape = 0;
		player2->dir = BBDIR_DROITE;
		player2->tire = false;

		if (type == LVL_BONUS) {
			p2_life = player2->nb_life;
			p2_bringBack = true;

			if (p2_life > 0) player2->nb_life = 1;
		}
	} else {
		p2_bringBack = false;
	}

	// Règlages conditionnels
	if (type == LVL_BONUS || type == LVL_COMPLETE || type == LVL_FIRST || type == LVL_END) {
		nb_ennemis_created = 0;

		if (player1 != NULL) {
			player1->setKilled(0);

			if (type != LVL_END) {
				player1->etat = ETAT_COME_BACK;
				player1->y = -50;
				player1->y_to_go = ystart1;
			}

			player1->id_arme = ID_M16;
			player1->latence_arme = 3;
			player1->nb_etape_arme = 5;
			player1->cadence_arme = 10;
			player1->poid_arme = 1;
			player1->perfect = true;
			player1->pv = 5;

			if (last_perfect1) {
				player1->setSuperWeapon();
			}
		}

		if (player2 != NULL) {
			player2->setKilled(0);

			if (type != LVL_END) {
				player2->etat = ETAT_COME_BACK;
				player2->y = -50;
				player2->y_to_go = ystart2;
			}

			player2->id_arme = ID_M16;
			player2->latence_arme = 3;
			player2->nb_etape_arme = 5;
			player2->cadence_arme = 10;
			player2->poid_arme = 1;
			player2->perfect = true;
			player2->pv = 5;

			if (last_perfect2) {
				player2->setSuperWeapon();
			}
		}
	}

	last_perfect1 = last_perfect2 = false;

	// Niveau bonus = temps limite
	niveau_bonus = type == LVL_BONUS;

	// Règle l'offset sur les joueurs
	Sprite* s = list_joueurs[0];
	offset = s->x;
	offset -= (offset % 640);

	n_img = 0;
	xTex = 0;
	n_cache = 0;
	next_x = 0;

	debug << "---------------------------------------------------------------\n";
	debug << "Now getting into game loop.\n";
	debug << "---------------------------------------------------------------\n";

	SDL_Delay(2000);

	if (briefing) {
		DDBLTFX ddfx{};
		Rect r;

		ddfx.dwSize = sizeof(ddfx);

		r.left = 0;
		r.right = 640;
		r.top = 0;
		r.bottom = 480;

		// backSurface->Blt(&r, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL,
		backSurface->FillRect(&r, 0);

		pbk_briefing[0]->PasteTo(backSurface, 0, 0);
		fnt_rpg.printC(backSurface, 320, 460, "Press a key to start.");
		DDFlip();

		mbk_inter.play(2);
		in.waitClean();
		in.waitKey();
		briefing = false;
	}

	// Update quelques trucs
	updateEvents();
	updateAll();

	// Pour le mode automatique
	// FIXME: what is this auto mode? should that step really be there?
	update_regulator_.Skip();

	// Boucle principale
	while (!joueurs_morts && !niveau_fini && !skipped && !app_killed) {
		gameLoop();
		updateVictoryAndDefeat();

		yieldToBrowser();
	}

	if (joueurs_morts) debug << "Players died\n";
	if (niveau_fini) debug << "Level complete\n";
	if (skipped) debug << "User skipped\n";
	if (app_killed) debug << "Application killed\n";

	if (player1 != NULL) {
		player1->endLevel();
	}

	if (player2 != NULL) {
		player2->endLevel();
	}

	// Coupe les musiques
	current_zik = -1;
	if (music_on) mbk_niveau.stop();

	// S'il s'agissait d'un niveau bonus on restaure les vies des
	// joueurs + on ne peut pas perdre
	if (type == LVL_BONUS && !skipped && !app_killed) {
		if (player1 != NULL && p1_bringBack) {
			if (player1->a_detruire && p1_life > 0) {
				player1->a_detruire = false;
				list_joueurs.push_back(player1.get());
			}

			player1->nb_life = p1_life;
		}

		if (player2 != NULL && p2_bringBack) {
			if (player2->a_detruire && p2_life > 0) {
				player2->a_detruire = false;
				list_joueurs.push_back(player2.get());
			}

			player2->nb_life = p2_life;
		}

		niveau_fini = true;
	}

	// Récapitulatif de fin de niveau
	if (niveau_fini && !joueurs_morts && type != LVL_END) {
		drawAll(false);
		DDFlip();
		drawAll(false);

		if ((type == LVL_BONUS && !skipped && game_flag[FLAG_TIMER] > 0) || type == LVL_COMPLETE || type == LVL_LAST) {
			if (player1 != NULL) {
				last_perfect1 = player1->perfect;
			}

			if (player2 != NULL) {
				last_perfect2 = player2->perfect;
			}

			showPE((type == LVL_BONUS));
		} else if (type == LVL_BONUS) {
			showPE(true, true);
		}
	} else if (type == LVL_BONUS) {
		showPE(true, true);
	}

	drawAll(false);
	drawLoading();
	DDFlipV();

	// Libére la mémoire
	releaseNiveau();

	return niveau_fini;
}

bool Game::chargeNiveau(const char* nom_niveau) {
	auto dirname = asset_path(nom_niveau);
	auto json_path = dirname + ".json";

	std::ifstream input(json_path);
	if (!input.good()) {
		debug << "Game::chargeNiveau() -> Cannot load <" << json_path << ">\n";
		return false;
	}

	json data = json::parse(input);

	debug << "---------------------------------------------------------------\n";
	debug << "Loading level <" << nom_niveau << ">\n";
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
				  load("gfx_niveau", "gfx/", "level stuff", false, [](const char* f) { return pbk_niveau.loadGFX(f); }) &&
				  load("gfx_ennemis", "gfx/", "ennemies", false, [](const char* f) { return pbk_ennemis.loadGFX(f); }) &&
				  load("sbk_ennemis", "sfx/", "SBK", false, [](const char* f) { return sbk_niveau.loadSFX(f); }) &&
				  load("fichier_mbk", "mbk/", "MBK", false,
					   [](const char* f) {
						   current_mbk = f;
						   return !music_on || mbk_niveau.open(f);
					   }) &&
				  load("fichier_rpg", "rpg/", "RPG file", false,
					   [&](const char* f) {
						   rpg.attachFile(f);
						   return true;
					   }) &&
				  load("gfx_rpg", "gfx/", "RPG GFX", false, [](const char* f) { return pbk_rpg.loadGFX(f); });
	if (!loaded) return false;
	if (data["fichier_mbk"].get<std::string>().empty()) current_mbk.clear();

	// Taille du niveau
	scr_level_size = data["num_decor"].size();
	level_size = scr_level_size * 640;

	auto plat_str = from_base64(data["y_plat"]);
	auto murs_opaques_str = from_base64(data["murs_opaques"]);
	auto murs_sanglants_str = from_base64(data["murs_sanglants"]);
	size_t level_size_8 = level_size / 8;
	if (plat_str.size() < NB_MAX_PLAT * level_size * sizeof(int32_t) || murs_opaques_str.size() < 60 * level_size_8 ||
		murs_sanglants_str.size() < 60 * level_size_8) {
		debug << "Game::chargeNiveau() -> Truncated collision data in <" << json_path << ">\n";
		return false;
	}

	// Numéros des écrans à afficher (comme des tiles)
	num_decor.resize(scr_level_size);
	for (size_t i = 0; i < scr_level_size; i++) {
		num_decor[i] = data["num_decor"][i];
	}

	// Coordonnées de départ des joueurs
	xstart1 = data["xstart1"];
	ystart1 = data["ystart1"];
	xstart2 = data["xstart2"];
	ystart2 = data["ystart2"];

	// Conditions de victoire
	vic_x = data["vic_x"];
	vic_flag1 = data["vic_flag1"];
	vic_val1 = data["vic_val1"];
	vic_flag2 = data["vic_flag2"];
	vic_val2 = data["vic_val2"];

	// Plateformes
	y_plat.assign(NB_MAX_PLAT, std::vector<int32_t>(level_size));
	for (int i = 0; i < NB_MAX_PLAT; i++) {
		std::memcpy(y_plat[i].data(), &plat_str[i * level_size * sizeof(int32_t)], level_size * sizeof(int32_t));
	}

	// Murs opaques
	murs_opaques.resize(60);
	for (size_t i = 0; i < 60; i++) {
		murs_opaques[i].assign(murs_opaques_str.begin() + i * level_size_8, murs_opaques_str.begin() + (i + 1) * level_size_8);
	}

	// Murs sanglants
	murs_sanglants.resize(60);
	for (size_t i = 0; i < 60; i++) {
		murs_sanglants[i].assign(murs_sanglants_str.begin() + i * level_size_8, murs_sanglants_str.begin() + (i + 1) * level_size_8);
	}

	// Charge les évenements
	size_t nb_events = data["events"].size();
	for (size_t i = 0; i < nb_events; i++) {
		auto event = data["events"][i];

		int event_id = event["event_id"];
		switch (event_id) {
			case EVENTID_ENNEMI: {
				auto event_ennemi = std::make_unique<EventEnemy>();

				event_ennemi->x_activation = event["x_activation"];
				event_ennemi->id_ennemi = event["id_ennemi"];
				event_ennemi->x = event["x"];
				event_ennemi->y = event["y"];
				event_ennemi->sens = event["sens"];

				list_event_endormis.push_back(std::move(event_ennemi));
				break;
			}

			case EVENTID_ENNEMI_GENERATOR: {
				auto event_gennemi = std::make_unique<EventEnemyGenerator>();

				event_gennemi->x_activation = event["x_activation"];
				event_gennemi->id_ennemi = event["id_ennemi"];
				event_gennemi->x = event["x"];
				event_gennemi->y = event["y"];
				event_gennemi->sens = event["sens"];
				event_gennemi->capacite = event["capacite"];
				event_gennemi->periode = event["periode"];
				event_gennemi->tmp = event["tmp"];

				list_event_endormis.push_back(std::move(event_gennemi));
				break;
			}

			case EVENTID_LOCK: {
				auto event_lock = std::make_unique<EventLock>();

				event_lock->x_activation = event["x_activation"];
				event_lock->cond = event["cond"];
				event_lock->flag = event["flag"];
				event_lock->val = event["val"];

				list_event_endormis.push_back(std::move(event_lock));
				break;
			}

			case EVENTID_FORCE_SCROLL: {
				auto event_scroll_speed = std::make_unique<EventScrollSpeed>();

				event_scroll_speed->x_activation = event["x_activation"];
				event_scroll_speed->speed = event["speed"];

				list_event_endormis.push_back(std::move(event_scroll_speed));
				break;
			}

			case EVENTID_FLAG: {
				auto event_set_flag = std::make_unique<EventSetFlag>();

				event_set_flag->x_activation = event["x_activation"];
				event_set_flag->flag = event["flag"];
				event_set_flag->val = event["val"];

				list_event_endormis.push_back(std::move(event_set_flag));
				break;
			}

			case EVENTID_HOLD_FIRE: {
				auto event_hold_fire = std::make_unique<EventHoldFire>();

				event_hold_fire->x_activation = event["x_activation"];
				event_hold_fire->flag = event["flag"];
				event_hold_fire->val = event["val"];

				list_event_endormis.push_back(std::move(event_hold_fire));
				break;
			}

			case EVENTID_BONUS_GENERATOR: {
				auto event_gen_bonus = std::make_unique<EventBonusGenerator>();

				event_gen_bonus->x_activation = event["x_activation"];
				event_gen_bonus->type = event["type"];
				event_gen_bonus->periode = event["periode"];

				list_event_endormis.push_back(std::move(event_gen_bonus));
				break;
			}

			case EVENTID_TEXT: {
				auto event_texte = std::make_unique<EventText>();

				event_texte->x_activation = event["x_activation"];
				event_texte->ntxt = event["ntxt"];
				event_texte->cond = event["cond"];
				event_texte->flag = event["flag"];
				event_texte->val = event["val"];

				list_event_endormis.push_back(std::move(event_texte));
				break;
			}

			case EVENTID_FOND_ANIME: {
				auto event_fond_anime = std::make_unique<EventAnimatedBackground>();

				event_fond_anime->x_activation = event["x_activation"];
				event_fond_anime->id_fond = event["id_fond"];
				event_fond_anime->x = event["x"];
				event_fond_anime->y = event["y"];

				list_event_endormis.push_back(std::move(event_fond_anime));
				break;
			}

			case EVENTID_MIFOND: {
				auto event_mi_fond = std::make_unique<EventMidground>();

				event_mi_fond->x_activation = event["x_activation"];
				event_mi_fond->id = event["id"];
				event_mi_fond->x = event["x"];
				event_mi_fond->y = event["y"];

				list_event_endormis.push_back(std::move(event_mi_fond));
				break;
			}

			case EVENTID_PREMIER_PLAN: {
				auto event_pplan = std::make_unique<EventForeground>();

				event_pplan->id_fond = event["id_fond"];
				event_pplan->x_activation = event["x_activation"];
				event_pplan->x = event["x"];
				event_pplan->y = event["y"];

				list_event_endormis.push_back(std::move(event_pplan));
				break;
			}

			case EVENTID_RPG: {
				auto event_rpg = std::make_unique<EventRPG>();

				event_rpg->x_activation = event["x_activation"];
				event_rpg->num = event["num"];
				event_rpg->cond = event["cond"];
				event_rpg->flag = event["flag"];
				event_rpg->val = event["val"];

				list_event_endormis.push_back(std::move(event_rpg));
				break;
			}

			case EVENTID_MUSIC: {
				auto event_music = std::make_unique<EventMusic>();

				event_music->x_activation = event["x_activation"];
				event_music->id = event["id"];
				event_music->play = event["play"];

				list_event_endormis.push_back(std::move(event_music));
				break;
			}

			case EVENTID_METEO: {
				auto event_meteo = std::make_unique<EventWeather>();

				event_meteo->x_activation = event["x_activation"];
				event_meteo->intensite = event["intensite"];
				event_meteo->type = event["type"];

				list_event_endormis.push_back(std::move(event_meteo));
				break;
			}

			case EVENTID_BONUS: {
				auto event_bonus = std::make_unique<EventBonus>();
				event_bonus->x_activation = event["x_activation"];
				event_bonus->type = event["type"];
				event_bonus->x = event["x"];
				event_bonus->y = event["y"];

				list_event_endormis.push_back(std::move(event_bonus));
				break;
			}

			case EVENTID_TURRET: {
				auto event_vehicule = std::make_unique<EventVehicle>();
				event_vehicule->x_activation = event["x_activation"];
				event_vehicule->id_vehicule = event["id_vehicule"];
				event_vehicule->x = event["x"];
				event_vehicule->y = event["y"];
				event_vehicule->dir = event["dir"];

				list_event_endormis.push_back(std::move(event_vehicule));
				break;
			}

			case EVENTID_SON: {
				auto event_son = std::make_unique<EventSound>();
				event_son->x_activation = event["x_activation"];
				event_son->nsnd = event["nsnd"];

				list_event_endormis.push_back(std::move(event_son));
				break;
			}
		}
	}

	debug << "Successfully loaded all level files\n";

	return true;
}

void Game::releaseNiveau() {
	num_decor.clear();
	y_plat.clear();
	murs_sanglants.clear();
	murs_opaques.clear();

	list_event_endormis.clear();
	list_event.clear();

	list_tirs_bb.clear();
	list_cow.clear();
	list_impacts.clear();

	list_vehicules.clear();

	list_ennemis.clear();
	list_tirs_ennemis.clear();
	list_gen_ennemis.clear();

	list_bonus.clear();
	list_gen_bonus.clear();

	list_txt_cool.clear();

	list_fonds_animes.clear();
	list_fonds_statiques.clear();
	list_premiers_plans.clear();
	list_plateformes_mobiles.clear();

	list_giclures.clear();
	list_gore.clear();

	list_meteo.clear();
	list_bulles.clear();
}

void Game::updateAll() {
	phase = !phase;

	if (phase) slow_phase = !slow_phase;

	// Met à jour les touches
	in.update();

	// Ok bonus ?
	okBonus = false;

	if (player1 != NULL) {
		okBonus = okBonus || player1->okBonus();
	}

	if (player2 != NULL) {
		okBonus = okBonus || player2->okBonus();
	}

	// Update tout ce qu'il faut
	if (!no_scroll1 && !no_scroll2) updateScrolling(false);

	updateEvents();

	// On update le scroll une deuxième fois à cause des Locks
	if (!no_scroll1 && !no_scroll2) updateScrolling();

	updateCheat();
	updateTremblements();

	if (game_flag[FLAG_BULLES]) updateBulles();

	if (type_meteo == METEO_PLUIE || type_meteo == METEO_NEIGE) updateMeteo();

	UpdateCollection(list_fonds_statiques);
	UpdateCollection(list_fonds_animes);
	UpdateCollection(list_plateformes_mobiles);
	UpdateCollection(list_vehicules);
	UpdateCollection(list_joueurs);
	UpdateCollection(list_tirs_bb);
	UpdateCollection(list_cow);
	UpdateCollection(list_impacts);
	UpdateCollection(list_ennemis);
	UpdateCollection(list_gore);
	UpdateCollection(list_tirs_ennemis);
	UpdateCollection(list_giclures);
	UpdateCollection(list_bonus);
	UpdateCollection(list_premiers_plans);

	UpdateCollection(list_txt_cool);
	UpdateCollection(list_gen_ennemis);
	UpdateCollection(list_gen_bonus);
	updateLock();
	updateHoldFire();
	updateTeteTurc();
	updateFlags();
	updateFlecheGo();

	if (type_meteo == METEO_DEFORME && intensite_meteo != 0) updateDeformation();

	manageCollisions();

	// Retire les trucs inutiles des listes
	cleanLists();

	updateMenu();

	updateRPG();
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
	DrawCollection(list_fonds_statiques);
	DrawCollection(list_fonds_animes);

	if (game_flag[FLAG_BULLES]) {
		DrawCollection(list_bulles);
	}

	DrawCollection(list_plateformes_mobiles);
	DrawCollection(list_impacts);
	DrawCollection(list_gore);

	DrawCollection(list_ennemis);
	DrawCollection(list_tirs_ennemis);
	DrawCollection(list_bonus);
	DrawCollection(list_tirs_bb);
	DrawCollection(list_joueurs);
	DrawCollection(list_vehicules);
	DrawCollection(list_impacts);
	DrawCollection(list_cow);
	DrawCollection(list_meteo);
	DrawCollection(list_premiers_plans);

	if (type_meteo == METEO_DEFORME && intensite_meteo != 0) drawDeformation();

	// FIXME: Disable it for now as it works unproperly at least on Linux
	drawHUB();
	drawTimer();
	go_.Draw();
	DrawCollection(list_txt_cool);

	drawDebugInfos();

	// Virer la bande qui déconne si besoin est
	if (vbuffer_wide == 640) {
		DDBLTFX ddfx{};
		Rect r;

		ddfx.dwSize = sizeof(ddfx);

		r.left = 638;
		r.right = 640;
		r.top = 0;
		r.bottom = 480;

		backSurface->/*Blt(&r, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddfx)*/
			FillRect(&r, 0);
	}

	Rect r;

	r.top = 0;
	r.left = 0;
	r.right = 640;
	r.bottom = 480;

	if (flip) {
		DDFlip();
	}
}

void Game::gameLoop() {
	int n_updates = update_regulator_.Step();
	for (; n_updates > 0; --n_updates) {
		updateAll();
	}

	drawAll();
}

void Game::releasePartie() { list_joueurs.clear(); }

bool Game::chargePartie() {
	// Charge les joueurs
	debug << "---------------------------------------------------------------\n";
	debug << "Loading global game files...\n";
	debug << "---------------------------------------------------------------\n";

	if (!pbk_blip.loadGFX("blip.gfx")) return false;

	debug << "Successfully loaded <blip.gfx>\n";

	if (!pbk_blop.loadGFX("blop.gfx")) return false;

	debug << "Successfully loaded <blop.gfx>\n";

	// Charge BB
	if (!pbk_bb.loadGFX("bb.gfx")) return false;

	debug << "Successfully loaded <bb.gfx>\n";

	// Charge Divers
	if (!pbk_misc.loadGFX("misc.gfx")) return false;

	debug << "Successfully loaded <misc.gfx>\n";

	// Charge gueules BB
	if (!pbk_rpg_bb.loadGFX("rpg_bb.gfx")) return false;

	debug << "Successfully loaded <rpg_bb.gfx>\n";

	// Font score blip
	if (!fnt_score_blip.load("scorei.lft")) return false;

	debug << "Successfully loaded <scorei.lft>\n";

	// Font score blop
	if (!fnt_score_blop.load("scoreo.lft")) return false;

	debug << "Successfully loaded <scoreo.lft>\n";

	// Font munitions
	if (!fnt_ammo.load("ammo1.lft")) return false;

	debug << "Successfully loaded <ammo1.lft>\n";

	// Font munitions utilisées
	if (!fnt_ammo_used.load("ammo2.lft")) return false;

	debug << "Successfully loaded <ammo2.lft>\n";

	// Sons globaux
	if (!sbk_misc.loadSFX("misc.sfx")) return false;

	debug << "Successfully loaded <misc.sfx>\n";

	// Sons BB
	if (!sbk_bb.loadSFX("bb.sfx")) return false;

	debug << "Successfully loaded <bb.sfx>\n";

	// La liste des niveaux
	if (!loadList("assets/bb.lst.json")) return false;

	debug << "Successfully loaded <bb.lst>\n";
	debug << "Successfully loaded all needed global game files\n";

	return true;
}

template <class T>
void Game::DrawCollection(const T& xs) {
	for (auto& pl : xs) {
		pl->affiche();
	}
}

template <class T>
void Game::RemoveDestroyed(T& xs) {
	xs.erase(std::remove_if(xs.begin(), xs.end(), [](auto& x) { return x->aDetruire(); }), xs.end());
}

void Game::cleanLists() {
	RemoveDestroyed(list_joueurs);
	RemoveDestroyed(list_fonds_statiques);
	RemoveDestroyed(list_tirs_bb);
	RemoveDestroyed(list_cow);
	RemoveDestroyed(list_bulles);
	RemoveDestroyed(list_impacts);
	RemoveDestroyed(list_ennemis);
	RemoveDestroyed(list_bonus);
	RemoveDestroyed(list_gen_ennemis);
	RemoveDestroyed(list_gen_bonus);
	RemoveDestroyed(list_txt_cool);
	RemoveDestroyed(list_fonds_animes);
	RemoveDestroyed(list_premiers_plans);
	RemoveDestroyed(list_giclures);
	RemoveDestroyed(list_tirs_ennemis);
	RemoveDestroyed(list_meteo);
	RemoveDestroyed(list_gore);
	RemoveDestroyed(list_plateformes_mobiles);
}

void Game::updateEvents() {
	// Les évenements de la liste "endormie" sont mis dans la liste "en attente"
	// quand ils sont sur le point d'être activés
	// FIXME: These reverse iterators are WEIRD. Why are the events coming
	// in reverse order? This feels so backward.
	for (auto it = list_event_endormis.rbegin(); it != list_event_endormis.rend(); ++it) {
		auto& event = *it;
		if (!event->aReveiller()) {
			break;
		}
		list_event.push_back(std::move(event));
	}

	list_event_endormis.erase(std::remove_if(list_event_endormis.begin(), list_event_endormis.end(), [](auto& ev) { return !ev.get(); }),
							  list_event_endormis.end());

	// Si les évenements "en attente" doivent être activés, on les active
	for (auto& event : list_event) {
		if (event->aActiver()) {
			event->doEvent();
			event.reset(nullptr);
		}
	}
	list_event.erase(std::remove_if(list_event.begin(), list_event.end(), [](auto& ev) { return !ev.get(); }), list_event.end());
}

void Game::manageCollisions() {
	// Collisions TirsBB / Ennemis
	for (Shot* tir : list_tirs_bb) {
		for (auto& ennemi : list_ennemis) {
			if (tir->collision(ennemi.get())) {
				ennemi->estTouche(tir);
			}
		}
	}

	// Collisions Vaches / Ennemis
	for (auto& tir : list_cow) {
		for (auto& ennemi : list_ennemis) {
			if (tir->collision(ennemi.get())) {
				ennemi->estTouche(tir.get());
			}
		}
	}

	// Collisions Joueurs / Bonus
	for (auto& bonus : list_bonus) {
		for (Ball* ball : list_joueurs) {
			if (bonus->collision(ball)) {
				bonus->estPris(ball);
			}
		}
	}

	if (wait_for_victory <= 0) {
		// Collisions Joueurs / Ennemis
		for (auto& ennemi : list_ennemis) {
			for (Ball* joueur : list_joueurs) {
				if (ennemi->collision(joueur)) joueur->estTouche(ennemi->degats());
			}
		}

		// Collisions Joueurs / tirs ennemis
		for (auto& tir : list_tirs_ennemis) {
			for (Ball* joueur : list_joueurs) {
				if (tir->collision(joueur)) {
					joueur->estTouche(tir->degats());
				}
			}
		}
	}
}

void Game::updateLock() {
	if (!scroll_locked) return;

	if ((cond_end_lock == 0 && list_ennemis.empty()) || (cond_end_lock == 1 && list_gen_ennemis.empty()) ||
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
		hud_.Draw(player1.get(), HUD::Location::Left, color, niveau_bonus);
	}

	if (player2 != NULL && player2->nb_life > 0) {
		auto color = player2->id_ball == ID_BLIP ? HUD::Color::Blue : HUD::Color::Orange;
		hud_.Draw(player2.get(), HUD::Location::Right, color, niveau_bonus);
	}
}

void Game::drawHUBpv(int x, int y, int pv) {
	static const int x_pv[] = {3, 0, 13, 44, 31};
	static const int y_pv[] = {0, 21, 48, 21, 0};

	for (int i = 0; i < pv; i++) pbk_bb[201 - i]->BlitTo(backSurface, x + x_pv[i], y + y_pv[i]);

	for (int i = pv; i < 5; i++) pbk_bb[196 - i]->BlitTo(backSurface, x + x_pv[i], y + y_pv[i]);
}

void Game::drawDebugInfos() {
	if (show_fps) {
		fnt_rpg.print(backSurface, 10, 130, ("FPS = " + std::to_string(fps_count)).c_str());
	}
}

void Game::updateTeteTurc() {
	static int ntete_turc = 0;

	if (list_joueurs.empty()) {
		dummyPlayer.x = offset + 320;
		dummyPlayer.y = y_plat[0][offset + 320];
		tete_turc = &dummyPlayer;
		return;
	}

	ntete_turc += 1;
	ntete_turc %= list_joueurs.size();
	tete_turc = list_joueurs[ntete_turc];
}

void Game::updateRPG() {
	if (rpg_to_play == -1) return;

	bool continued = true;

	rpg.startPlay(rpg_to_play);

	while (continued && !app_killed) {
		drawAll(false);
		continued = rpg.drawScene(backSurface);
		DDFlipV();

		yieldToBrowser();
	}

	rpg.stopPlay();
	in.waitClean();
	rpg_to_play = -1;
	update_regulator_.Skip();
}

void Game::updateVictoryAndDefeat() {
	// Defaite ?
	if (list_joueurs.empty()) {
		wait_for_death += 1;

		if (wait_for_death >= 200) joueurs_morts = true;

		return;
	}

	// Victoire ?
	if (offset >= vic_x && game_flag[vic_flag1] == vic_val1 && game_flag[vic_flag2] == vic_val2) {
		hold_fire = true;
		wait_for_victory += 1;

		if (game_flag[1] == 999) wait_for_victory = 200;

		if (wait_for_victory >= 200) niveau_fini = true;
	} else {
		if (wait_for_victory > 0) wait_for_victory = 0;
	}
}

void Game::updateFlags() {
	game_flag[FLAG_NB_GEN] = (int)list_gen_ennemis.size();
	game_flag[FLAG_NB_ENN] = (int)list_ennemis.size();
	makeb_current_mode = game_flag[FLAG_BONUS];

	// Le TIMER
	if (game_flag[FLAG_TIMER] > 0 && wait_for_victory == -1) {
		etape_timer += 1;
		etape_timer %= 80;

		if (etape_timer == 0) {
			game_flag[FLAG_TIMER] -= 1;

			if (game_flag[FLAG_TIMER] == 0 && niveau_bonus) niveau_fini = true;
		}
	}
}

void Game::drawTimer() {
	if (game_flag[FLAG_TIMER] > 0) {
		fnt_cool.printC(backSurface, 320, 20, txt_data[TXT_TIME]);
		fnt_cool.printC(backSurface, 320, 50, std::to_string(game_flag[FLAG_TIMER]));
	}
}

void Game::updateMenu() {
	// Appelle le menu s'il y a lieu de le faire
	if (in.scanKey(DIK_ESCAPE)) {
		MenuGame menu;
		int r;

		drawAll(false);
		DDFlipV();
		drawAll(false);
		DDFlipV();

		menu.Draw(backSurface);
		DDFlipV();

		in.waitClean();

		while (!(r = menu.Update()) && !app_killed) {
			drawAll(false);
			menu.Draw(backSurface);

			DDFlipV();

			yieldToBrowser();
		}

		if (r == 2) skipped = true;

		drawAll(false);
		DDFlipV();
		drawAll(false);
		DDFlipV();

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

	if (in.scanKey(DIK_F9)) niveau_fini = true;

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

void Game::showPE(bool bonus, bool fuckOff) {
	PlayerTotals totals[2];
	totals[0].player = player1.get();
	totals[1].player = player2.get();

	systemSurface->BltFast(0, 0, backSurface, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

	Rect r;
	r.top = 100;
	r.left = 60;
	r.right = 580;
	r.bottom = 400;
	halfTone(systemSurface, &r);

	mbk_inter.play(0);

	if (list_joueurs.size() == 2) {
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
		if (nb_ennemis_created > 0) t.killed = (100 * t.player->getKilled()) / nb_ennemis_created;
		if (!bonus || (game_flag[FLAG_TIMER] > 0 && !joueurs_morts)) t.obj = 50000;
		if (t.obj > 0 && t.player->perfect) t.perfect = 50000;
		t.total = t.player->getScore();
		if (fuckOff) t.perfect = t.obj = t.killed = 0;
	}
	if (fuckOff) game_flag[FLAG_TIMER] = 0;

	int delai = 0;

	// Runs at least once so the screen shows even when no player is left to tally
	do {
		in.update();

		delai += 1;
		delai %= 3;

		if (in.anyKeyPressed()) delai = 0;

		if (delai == 0) {
			for (auto& t : totals) t.step(game_flag[FLAG_TIMER]);
		}

		backSurface->BltFast(0, 0, systemSurface, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

		fnt_cool.printC(backSurface, 320, 120, fuckOff ? "BONUS STAGE FAILED!" : "LEVEL COMPLETE!");

		for (auto& t : totals) {
			if (t.fnt) t.draw(game_flag[FLAG_TIMER]);
		}

		DDFlipV();

		yieldToBrowser();
	} while (!app_killed && (totals[0].calc < 4 || totals[1].calc < 4));

	if (!app_killed) {
		in.waitClean();
		in.waitKey();
	}

	for (auto& t : totals) {
		if (t.fnt) t.player->setScore(t.total);
	}

	mbk_inter.stop();
}

void Game::updateMeteo() {
	const static int speed_gouttes[] = {3, 5, 7, 8, 9, 10};

	for (auto& pl : list_meteo) {
		pl->update();
	}

	while (list_meteo.size() < (size_t)intensite_meteo) {
		if (type_meteo == METEO_NEIGE) {
			next_flocon = (next_flocon + 1) % NB_FLOCONS;
			WeatherSnow* flocon = &neige[next_flocon];

			int d = rand() % 3;

			flocon->a_detruire = false;
			flocon->xbase = offset + rand() % 800;
			flocon->y = rand() % 480 - 500;

			if (d <= 1)
				flocon->pic = pbk_misc[71];
			else
				flocon->pic = pbk_misc[72];

			flocon->dy = intensite_meteo / 20 + d;
			flocon->phi = rand() % 360;
			flocon->xwide = 10 + d * 4;

			if (mur_opaque(flocon->xbase, 0)) flocon->y -= 550;

			list_meteo.emplace_back(flocon);
		} else if (type_meteo == METEO_PLUIE) {
			next_goutte = (next_goutte + 1) % NB_GOUTTES;
			WeatherRain* goutte = &pluie[next_goutte];

			int d = rand() % 4;

			goutte->a_detruire = false;
			goutte->x = offset + rand() % 800;
			goutte->y = rand() % 480 - 480;
			goutte->pic = pbk_misc[65 + d];
			goutte->dy = 7 + d * 2;

			if (mur_opaque(goutte->x, 0)) goutte->y -= 550;

			list_meteo.emplace_back(goutte);
		}
	}
}

template <class T>
void Game::UpdateCollection(const T& xs) {
	for (auto& pl : xs) {
		pl->update();
	}
}

void Game::updateFlecheGo() {
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
	DDBLTFX ddfx{};
	Rect r;
	int pas = 20;
	int phi = phi_deform;
	int dphi = 10;
	int x;
	int xt;

	ddfx.dwSize = sizeof(ddfx);

	pas = 2;
	dphi = 1;

	for (int y = 0; y < 480; y += pas) {
		phi += dphi;
		phi %= 360;

		r.top = y;
		r.bottom = y + pas;

		x = xt = sini(5, phi);

		// FIXME: This is absolutely wrong. We are copying a memory location to
		// another that is overlapping (moving a line a few pixels). Depending
		// on the order of overlap, we might be (and are) overwriting the data
		// we're copying from and screwing everything up.  A temp satisfying
		// fix would involve allocating a third surface to safely copy or doing
		// it ourselves by locking the surface into host memory. A long term
		// and much better fix is using pixel shaders.

		if (x < 0) {
			r.left = -x;
			r.right = 640;
			x = 0;
		} /*else {
			// I'm overwriting my own mem!
			r.left = 0;
			r.right = 640 - x;
		}*/

		backSurface->BltFast(x, y, backSurface, &r, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);

		if (xt < 0) {
			r.left = 640 + xt;
			r.right = 640;
		} else {
			r.left = 0;
			r.right = xt;
		}

		backSurface->Blt(&r, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddfx);
	}
}

void Game::updateBulles() {
	// Crée de nouvelles bulles
	for (Ball* pl : list_joueurs) {
		creeBulle(pl);
	}

	for (auto& pl : list_ennemis) {
		if (list_bulles.size() < 15) {
			break;
		}
		creeBulle(pl.get());
	}

	// Update les bulles déjà crées
	for (auto& pl : list_bulles) {
		pl->update();
	}
}

void Game::creeBulle(Sprite* s) {
	s->wait_bulle += rand() % 10;

	if (s->wait_bulle > 300) {
		Bubble* b = new Bubble();

		b->xbase = s->x;
		b->y = s->y - 10;
		b->pic = pbk_misc[78 + rand() % 3];
		b->dphi = 5 + rand() % 3;
		b->dy = -(2 + rand() % 2);

		list_bulles.emplace_back(b);

		s->wait_bulle = 0;
	}
}

void Game::updateTremblements() {
	if (amplitude_tremblement == 0) return;

	if (etape_tremblement == 0) {
		dy_tremblement += ddy_tremblement;

		if (dy_tremblement <= -amplitude_tremblement) {
			amplitude_tremblement -= 1;
			ddy_tremblement = (amplitude_tremblement >> 1) + 1;
			dy_tremblement += 1;
		} else if (dy_tremblement >= amplitude_tremblement) {
			amplitude_tremblement -= 1;
			ddy_tremblement = -(amplitude_tremblement >> 1) - 1;
			dy_tremblement -= 1;
		}
	}
}

void Game::drawTremblements() {
	if (amplitude_tremblement == 0 || dy_tremblement == 0) return;

	int y = 0;
	Rect r;
	Rect r2;

	r2.left = r.left = 0;
	r2.right = r.right = 640;

	if (dy_tremblement < 0) {
		r.top = -dy_tremblement;
		r.bottom = 480;

		r2.top = 480 + dy_tremblement;
		r2.bottom = 480;

		y = 0;
	} else if (dy_tremblement > 0) {
		r.top = 0;
		r.bottom = 480 - dy_tremblement;

		r2.top = 0;
		r2.bottom = dy_tremblement;

		y = dy_tremblement;
	}

	backSurface->BltFast(0, y, backSurface, &r, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);

	DDBLTFX ddfx{};
	ddfx.dwSize = sizeof(ddfx);

	backSurface->Blt(&r2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddfx);
}

bool Game::loadList(const char* fic) {
	std::ifstream input(fic);
	if (!input.good()) {
		debug << "Cannot open " << fic << "\n";
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

		fic_names[nb_part] = item["path"].get<std::string>();

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

void Game::getHiscore() {
	if (player1 != NULL && hi_scores.isGood(player1->getScore())) getName(player1.get(), 1);

	if (player2 != NULL && hi_scores.isGood(player2->getScore())) getName(player2.get(), 2);
}

void Game::getName(Player* joueur, int ijoueur) {
	const std::string title = "PLAYER " + std::to_string(ijoueur) + " GOT A HIGH SCORE!";
	std::string name;
	int key = -1;
	int x = 640;

	while (!app_killed && x >= 40) {
		in.update();
		x -= 20;

		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		fnt_menu.printC(backSurface, 320 - x, 160, title);
		fnt_menu.printC(backSurface, 320 + x, 210, "PLEASE ENTER YOUR NAME :");

		DDFlipV();

		yieldToBrowser();
	}

	while (!app_killed && key != DIK_RETURN) {
		in.waitClean();
		key = in.waitKey();
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
		DDFlipV();

		yieldToBrowser();
	}

	while (!app_killed && x < 640) {
		x += 20;

		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		fnt_menu.printC(backSurface, 320 - x, 160, title);
		fnt_menu.printC(backSurface, 320 + x, 210, "PLEASE ENTER YOUR NAME :");
		fnt_menu.printC(backSurface, 320, 260 + x, name);

		DDFlipV();	// primSurface->Flip( NULL, 0);

		yieldToBrowser();
	}

	hi_scores.add(joueur->getScore(), name);
	save_settings();
}

void Game::showGameOver() {
	Chrono t;

	mbk_inter.play(1);

	while (!app_killed && t.elapsed() <= 5000 && !in.anyKeyPressed()) {
		pbk_inter[0]->PasteTo(backSurface, 0, 0);
		DDFlipV();	// primSurface->Flip( NULL, 0);

		yieldToBrowser();
	}
	mbk_inter.stop();
}

void Game::showHighScores() {
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

		DDFlipV();	// primSurface->Flip( NULL, 0);
		yieldToBrowser();
	}

	while (!app_killed && x[HS_NB_SCORES - 1] < 350) {
		in.update();
		pbk_inter[1]->PasteTo(backSurface, 0, 0);

		for (int i = 0; i < HS_NB_SCORES; i++) {
			if (i == 0 || x[i - 1] >= 160) x[i] += 20;

			fnt_cool.printR(backSurface, 300 - x[i], 160 + 30 * i, std::to_string(hi_scores.getScore(i)).c_str());

			fnt_cool.print(backSurface, 320 + x[i], 160 + 30 * i, hi_scores.getName(i));
		}

		DDFlipV();	// primSurface->Flip( NULL, 0);
		yieldToBrowser();
	}

	in.waitClean();
}

void Game::go() {
	TitleScreen menu;
	int r;
	static int attract_step = 0;

	// Clear l'écran en noir + LOADING
	{
		DDBLTFX ddfx{};
		ddfx.dwSize = sizeof(ddfx);

		backSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddfx);
		fnt_cool.printC(backSurface, 320, 240, "PLEASE WAIT");
	}

	CINEPlayer cine;

	cine.loadPBK("intro.gfx");
	mbk_interl.play(0);
	cine.playScene("intro.cin", primSurface, backSurface);

	menu.start();
	in.waitClean();

	Chrono diff_start;

	do {
		r = RET_CONTINUE;

		while (!r && !app_killed) {
			in.update();

			if (in.anyKeyPressed()) {
				diff_start.Reset();
			}

			if (diff_start.elapsed() >= 10000) {
				switch (attract_step) {
					case 0:
					case 2:
					case 4:
					case 6:
						showMainScreen();
						break;

					case 1:
						showHighScores();
						break;

					case 5:
						showCredits();
						break;

					case 3:
					case 7:
						cine.loadPBK("intro.gfx");
						cine.playScene("intro.cin", primSurface, backSurface);
						break;
				}

				attract_step = (attract_step + 1) % 8;

				menu.stop();
				menu.start();

				if (in.anyKeyPressed()) {
					diff_start.Reset();
					in.waitClean();
				}
			} else {
				pbk_inter[1]->PasteTo(backSurface, 0, 0);

				r = menu.update();
				menu.draw(backSurface);

				DDFlipV();
			}

			yieldToBrowser();
		}

		if (r == RET_START_GAME1 || r == RET_START_GAME2) {
			CharacterSelection select;

			auto selected = CharacterSelection::Output::Continue;
			while (selected == CharacterSelection::Output::Continue && !app_killed) {
				in.update();
				selected = select.update();
				select.draw();
				DDFlipV();

				yieldToBrowser();
			}
			// The confirming pad A is also the default jump
			in.waitClean();

			drawLoading();
			DDFlipV();

			if (r == RET_START_GAME1)
				jouePartie(1, int(selected));
			else
				jouePartie(2, int(selected));

			menu.stop();
			menu.start();
			mbk_interl.play(0);
			diff_start.Reset();
		}

		yieldToBrowser();

	} while (r != RET_EXIT && !app_killed);

	menu.stop();

	mbk_interl.stop();
}

void Game::showBriefing(const char* fn) {
	pbk_briefing.loadGFX(fn);

	briefing = true;

	DDBLTFX ddfx{};
	Rect r;

	ddfx.dwSize = sizeof(ddfx);

	r.left = 0;
	r.right = 640;
	r.top = 0;
	r.bottom = 480;

	backSurface->Blt(&r, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddfx);

	pbk_briefing[0]->PasteTo(backSurface, 0, 0);
	fnt_rpg.printC(backSurface, 320, 460, "Loading...");
	DDFlip();
}

void Game::showMainScreen() {
	Chrono t;

	while (!in.anyKeyPressed() && !app_killed && t.elapsed() <= 5000) {
		in.update();

		pbk_inter[1]->PasteTo(backSurface, 0, 0);
		DDFlipV();	// primSurface->Flip(NULL, DDFLIP_WAIT );

		yieldToBrowser();
	}
}

void Game::showCredits(bool theEnd) {
	static const int NB_PAGE = 6;
	static const int ILIGNE = 20;
	static const int ITITRE = 40;
	static const int IPARTI = 120;
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

		in.update();

		{
			DDBLTFX ddfx{};
			Rect r;

			ddfx.dwSize = sizeof(ddfx);

			r.left = 0;
			r.right = 640;
			r.top = 0;
			r.bottom = 480;

			backSurface->Blt(&r, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddfx);
		}

		if (theEnd) {
			pbk_cred[npage]->PasteTo(backSurface, 0, 0);
		} else {
			pbk_inter[1]->BlitTo(backSurface, 0, 0);
		}

		fnt_rpg.printC(backSurface, xcred, y + ILIGNE, "Credits");

		int y1 = y + ILIGNE + IPARTI;

		fnt_rpg.printC(backSurface, xcred, y1, "CODE");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 0, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 1, "Sylvain Bugat");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 3, "2019 REVAMP");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 4, "Guillaume Sanchez");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 6, "Web port");
		fnt_rpg.printC(backSurface, xcred, y1 + ITITRE + ILIGNE * 7, "Adam Kecskes");

		int y2 = y1 + 4 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y2, "MUSIC AND SOUND");
		fnt_rpg.printC(backSurface, xcred, y2 + ILIGNE, "DESIGN");
		fnt_rpg.printC(backSurface, xcred, y2 + ITITRE + ILIGNE, "Gerard Kelly");

		int y3 = y2 + 2 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y3, "ADDITIONAL MUSIC");
		fnt_rpg.printC(backSurface, xcred, y3 + ITITRE + 0 * ILIGNE, "El Mobo");
		fnt_rpg.printC(backSurface, xcred, y3 + ITITRE + 1 * ILIGNE, "Rik Ede");

		int y4 = y3 + 2 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y4, "ARTWORK");
		fnt_rpg.printC(backSurface, xcred, y4 + ITITRE + ILIGNE * 0, "Laurent Schneider");
		fnt_rpg.printC(backSurface, xcred, y4 + ITITRE + ILIGNE * 1, "Jérémie Comarmond");
		fnt_rpg.printC(backSurface, xcred, y4 + ITITRE + ILIGNE * 2, "Jérôme Karaban");
		fnt_rpg.printC(backSurface, xcred, y4 + ITITRE + ILIGNE * 3, "Didier Colin");
		fnt_rpg.printC(backSurface, xcred, y4 + ITITRE + ILIGNE * 4, "Sylvain Bugat");

		int y5 = y4 + 5 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y5, "ADDITIONAL SOUND");
		fnt_rpg.printC(backSurface, xcred, y5 + ILIGNE, "AND VOCAL EFFECTS");
		fnt_rpg.printC(backSurface, xcred, y5 + ITITRE + 1 * ILIGNE, "Eric Khodja");
		fnt_rpg.printC(backSurface, xcred, y5 + ITITRE + 2 * ILIGNE, "Jérôme Karaban");
		fnt_rpg.printC(backSurface, xcred, y5 + ITITRE + 3 * ILIGNE, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y5 + ITITRE + 4 * ILIGNE, "Vincent Lagarrigue");

		int y6 = y5 + 5 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y6, "STORYLINE");
		fnt_rpg.printC(backSurface, xcred, y6 + ITITRE + 0 * ILIGNE, "Benjamin Karaban");
		fnt_rpg.printC(backSurface, xcred, y6 + ITITRE + 1 * ILIGNE, "Eric Khodja");

		int y7 = y6 + 2 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y7, "ORIGINAL CONCEPT");
		fnt_rpg.printC(backSurface, xcred, y7 + ITITRE + 0 * ILIGNE, "Laurent Schneider");
		fnt_rpg.printC(backSurface, xcred, y7 + ITITRE + 1 * ILIGNE, "Jérémie Comarmond");
		fnt_rpg.printC(backSurface, xcred, y7 + ITITRE + 2 * ILIGNE, "Didier Colin");
		fnt_rpg.printC(backSurface, xcred, y7 + ITITRE + 3 * ILIGNE, "Sylvain Bugat");
		fnt_rpg.printC(backSurface, xcred, y7 + ITITRE + 4 * ILIGNE, "Benjamin Karaban");

		int y8 = y7 + 5 * ILIGNE + IPARTI + ITITRE;

		fnt_rpg.printC(backSurface, xcred, y8, "BETA TEST");
		fnt_rpg.printC(backSurface, xcred, y8 + ITITRE + 0 * ILIGNE, "Jérémie Khodja");
		fnt_rpg.printC(backSurface, xcred, y8 + ITITRE + 1 * ILIGNE, "Julien Areas");
		fnt_rpg.printC(backSurface, xcred, y8 + ITITRE + 2 * ILIGNE, "Carlos Sédille");
		fnt_rpg.printC(backSurface, xcred, y8 + ITITRE + 3 * ILIGNE, "Juliette Karaban");

		int y9 = y8 + 4 * ILIGNE + IPARTI + ITITRE;
		int y10 = y9;
		int y11 = y10;

		if (theEnd) {
			fnt_rpg.printC(backSurface, xcred, y9, "THANKS TO");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 0 * ILIGNE, "Arnaud Dubois");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 1 * ILIGNE, "Carole Schmitt");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 2 * ILIGNE, "Aasterion staff");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 3 * ILIGNE, "Aurore Anger");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 4 * ILIGNE, "Cécile Baudoncourt");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 5 * ILIGNE, "Nicolas Clément");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 6 * ILIGNE, "Nathalie Morin");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 7 * ILIGNE, "Florent and Thomas");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 8 * ILIGNE, "Bruno Singer");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 9 * ILIGNE, "Francois Deschamps");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 10 * ILIGNE, "Colin Auger");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 11 * ILIGNE, "TP122 students");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 12 * ILIGNE, "Jeff");
			fnt_rpg.printC(backSurface, xcred, y9 + ITITRE + 13 * ILIGNE, "Flavy");

			y10 = y9 + 14 * ILIGNE + IPARTI + ITITRE;

			fnt_rpg.printC(backSurface, xcred, y10, "SPECIAL THANKS TO");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 0 * ILIGNE, "La Guinguette Pirate");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 1 * ILIGNE, "TiPunch power!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 3 * ILIGNE, "La Pirogue");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 4 * ILIGNE, "I love you Séverine!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 6 * ILIGNE, "Le Montbauron");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 7 * ILIGNE, "mind the 8 ball!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 9 * ILIGNE, "La Fleche d'Or");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 10 * ILIGNE, "avoid the toilets!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 12 * ILIGNE, "Hippochine Restaurant");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 13 * ILIGNE, "great music!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 15 * ILIGNE, "Vitigno Pizzeria");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 16 * ILIGNE, "pizza! good!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 18 * ILIGNE, "Max Linder Theatre");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 19 * ILIGNE, "THX rules!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 21 * ILIGNE, "UNEF");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 22 * ILIGNE, "cheap beer 4 all!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 24 * ILIGNE, "Hayao Myazaki");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 25 * ILIGNE, "Totoro! To-to-ro!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 27 * ILIGNE, "Kevin Smith");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 28 * ILIGNE, "thirty seven?");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 30 * ILIGNE, "Peter Jackson");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 31 * ILIGNE, "Bad Taste rules!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 33 * ILIGNE, "Monty Python");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 34 * ILIGNE, "behind the rabbit?");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 36 * ILIGNE, "John Boorman");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 37 * ILIGNE, "Zardoz has spoken!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 39 * ILIGNE, "Luc Besson");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 40 * ILIGNE, "nah... just kidding!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 42 * ILIGNE, "Christopher Lambert");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 43 * ILIGNE, "watch your back!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 45 * ILIGNE, "Jeremy Irons");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 46 * ILIGNE, "lord Profion!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 48 * ILIGNE, "Wayne Scott");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 49 * ILIGNE, "Rambo! Rambo!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 51 * ILIGNE, "Les Joyeux Urbains");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 52 * ILIGNE, "va chez ta mère!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 54 * ILIGNE, "Toss");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 55 * ILIGNE, "da! da! da!");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 57 * ILIGNE, "Le cassoulet");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 58 * ILIGNE, "best served at 4 AM");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 60 * ILIGNE, "Gamedev.net");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 61 * ILIGNE, "DirectX vs hamsters");

			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 64 * ILIGNE, "THANK");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 65 * ILIGNE, "YOU");
			fnt_rpg.printC(backSurface, xcred, y10 + ITITRE + 66 * ILIGNE, "FOR PLAYING");

			y11 = y10 + 72 * ILIGNE + IPARTI + ITITRE;
		}

		fnt_rpg.printC(backSurface, xcred, y11, "LOADED Studio");
		fnt_rpg.printC(backSurface, xcred, y11 + ILIGNE, "May 2002");

		last_y = y11 + 2 * ILIGNE;

		if (t.elapsed() >= 12500 && theEnd) {
			npage++;
			npage %= pbk_cred.getSize();
			t.Reset();
		}

		DDFlip();
		yieldToBrowser();
	}
}
