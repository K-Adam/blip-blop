#pragma once

#include <iomanip>
#include <sstream>

#include "entities/ball.h"
#include "level/world.h"
#include "core/assets.h"

class HUD {
   public:
	enum class Location : int { Left, Right };
	enum class Color { Blue, Orange };

	void Draw(Ball* player, Location location, Color color, bool bonus_stage) {
		DrawHealth(player->hp, location);
		DrawScore(player->getScore(), location, color);
		if (bonus_stage) {
			DrawBonusStage(location);
		} else {
			DrawLives(player->nb_life, location);
		}
		DrawWeapon(player, location);

		if (player->hasUnlimitedAmmo()) {
			DrawAmmo(-1, false, location);
		} else {
			DrawAmmo(player->ammo[player->weapon_id], player->is_firing, location);
		}
		DrawCowBomb(player->nb_cow_bomb, location);
		DrawInventory(player, location);
	}

   private:
	void DrawCowBomb(int nb, Location location) {
		if (location == Location::Left) {
			for (int i = 0, dxt = 0; i < nb; i++, dxt += 23) pbk_misc.pic("icon_cow_bomb")->BlitTo(backSurface, 90 + dxt, 65);
		} else {
			for (int i = 0, dxt = 0; i < nb; i++, dxt += 23) pbk_misc.pic("icon_cow_bomb")->BlitTo(backSurface, 520 - dxt, 65);
		}
	}

	void DrawAmmo(int ammo, bool firing, Location location) {
		int x = location == Location::Left ? 135 : 505;
		if (ammo == -1) {
			fnt_ammo.printC(backSurface, x, 43, "*");
			return;
		}

		std::string ammo_str = std::to_string(ammo);
		if (firing) {
			fnt_ammo_used.printC(backSurface, x, 43, ammo_str);
		} else {
			fnt_ammo.printC(backSurface, x, 43, ammo_str);
		}
	}

	void DrawInventory(const Ball* player, Location location) {
		// Grows toward the middle of the screen, away from the weapon in hand
		int x = location == Location::Left ? 190 : 455;
		int dx = location == Location::Left ? 36 : -36;
		for (int id = 0; id < NB_WEAPONS; id++) {
			if (id == player->weapon_id || !player->carries(id)) continue;
			WeaponIcon(player, id)->BlitScaled(backSurface, x, 50, 0.5);
			x += dx;
		}
	}

	void DrawWeapon(const Ball* player, Location location) {
		int x = (location == Location::Left) ? 135 : 510;
		WeaponIcon(player, player->weapon_id)->BlitTo(backSurface, x, 50);
	}

	static const Picture* WeaponIcon(const Ball* player, int id) { return pbk_misc.pic(player->weaponAt(id).stats.icon); }

	void DrawBonusStage(Location location) {
		if (location == Location::Left) {
			fnt_rpg.print(backSurface, 4, 5, "Bonus");
			fnt_rpg.print(backSurface, 4, 20, "Stage");
		} else {
			fnt_rpg.printR(backSurface, 635, 5, "Bonus");
			fnt_rpg.printR(backSurface, 635, 20, "Stage");
		}
	}

	void DrawLives(int lives, Location location) {
		if (location == Location::Left) {
			fnt_cool.print(backSurface, 5, 5, std::to_string(lives));
		} else {
			fnt_cool.printR(backSurface, 635, 5, std::to_string(lives));
		}
	}

	void DrawScore(int score, Location location, Color color) {
		std::ostringstream oss;
		oss << std::setw(7) << std::setfill('0') << score;
		std::string s = oss.str();

		int x = (location == Location::Left) ? 100 : 450;
		if (color == Color::Blue) {
			fnt_score_blip.print(backSurface, x, 20, s);
		} else {
			fnt_score_blop.print(backSurface, x, 20, s);
		}
	}

	static void DrawHealth(int health, Location location) {
		static const int x_health[] = {3, 0, 13, 44, 31};
		static const int y_health[] = {0, 21, 48, 21, 0};
		static const int pos[2][2] = {{20, 20}, {560, 20}};

		int x = pos[int(location)][0];
		int y = pos[int(location)][1];

		for (int i = 0; i < health; i++) {
			pbk_bb.anim("health_full")[4 - i]->BlitTo(backSurface, x + x_health[i], y + y_health[i]);
		}

		for (int i = health; i < 5; i++) {
			pbk_bb.anim("health_empty")[4 - i]->BlitTo(backSurface, x + x_health[i], y + y_health[i]);
		}
	}
};
