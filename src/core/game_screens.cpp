#include "core/game.h"

#include "cinematics/cine_player.h"
#include "core/app.h"
#include "core/debug.h"
#include "engine/half_tone.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/character_selection.h"
#include "menus/join_screen.h"
#include "menus/title_screen.h"

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
Task<> getName(Player* hero, int player_num) {
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

	while (!in.quitRequested() && key != SDLK_RETURN) {
		co_await in.waitClean();
		key = co_await in.waitKey();
		if (in.scanKey(SDLK_RETURN)) key = SDLK_RETURN;

		std::string key_tmp = keyToString(key, settings().lang == LANG_FR);

		if (key == SDLK_SPACE && name.size() < 19) name += ' ';
		if (key == SDLK_BACKSPACE && !name.empty()) {
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

Task<> showMainScreen() {
	Chrono t;

	while (!in.anyKeyPressed() && !in.quitRequested() && t.elapsed() <= 5000) {
		assets().pics.inter.pic("title")->PasteTo(graphics.back(), 0, 0);
		graphics.Flip();

		co_await nextFrame();
	}
}
}  // namespace

Task<> Game::showPE(bool bonus, bool bonus_failed) {
	PlayerTotals totals[2];
	totals[0].player = player1.get();
	totals[1].player = player2.get();

	graphics.spare()->BltFast(0, 0, graphics.back(), nullptr);

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
	} else if (player1 != nullptr && player1->nb_life > 0) {
		totals[0].calc = 0;
		totals[0].xbase = 210;
	} else if (player2 != nullptr) {
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

		graphics.back()->BltFast(0, 0, graphics.spare(), nullptr);

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

void Game::drawLoading() { assets().fonts.cool.printC(graphics.back(), 320, 205, "LOADING"); }

Task<> Game::getHiscore() {
	if (player1 != nullptr && hiScores().isGood(player1->getScore())) co_await getName(player1.get(), 1);

	if (player2 != nullptr && hiScores().isGood(player2->getScore())) co_await getName(player2.get(), 2);
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

				co_await playGame(join.players(), static_cast<int>(selected));
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
