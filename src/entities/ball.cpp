#include "core/config.h"
#include "entities/ball.h"
#include "entities/shots/shot_bb_m16.h"
#include "entities/shots/shot_bb_pm.h"
#include "entities/shots/shot_bb_shotgun.h"
#include "entities/shots/shot_bb_laser.h"
#include "entities/shots/shot_bb_cow.h"

#include <algorithm>
#include <cmath>
#include <numbers>

const int anim_blip_marche_droite[] = {0, 1, 2, 1};
const int anim_blip_marche_gauche[] = {3, 4, 5, 4};

const int anim_blip_saute_droite[] = {8, 8, 8, 7, 7, 6, 6, 0, 0, 0, 0};
const int anim_blip_saute_gauche[] = {11, 11, 11, 10, 10, 9, 9, 3, 3, 3, 3};

const int anime_sautille[] = {0, 2, 4, 2};

const int anim_m16_deto[] = {0, 1, 2, 1, 0};
const int anim_pm_deto[] = {0, 1, 2, 1, 0};
const int anim_fusil_deto[] = {0, 1, 2, 1, 0};

const int dx_m16[] = {3, 6, 0, 2, 7, -7, -2, 0, -6, -3};
const int dy_m16[] = {-15, -9, -12, -10, -10, -10, -10, -12, -9, -15};

const int dx_pm[] = {2, 6, 0, 0, 7, -7, 0, 0, -6, -2};
const int dy_pm[] = {-12, -6, -10, -15, -9, -9, -15, -10, -6, -12};

const int dx_lf[] = {-10, -10, -10, -10, -10, 10, 10, 10, 10, 10};
const int dy_lf[] = {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10};

const int dx_fusil[] = {1, -4, -6, -4, -4, 4, 4, 6, 4, -1};
const int dy_fusil[] = {-14, -12, -11, -10, -9, -9, -10, -11, -12, -14};

const int dx_main_fusil[] = {0, 17, 24, 18, -3, 3, -18, -24, -17, 0};
const int dy_main_fusil[] = {16, 10, -10, -31, -40, -38, -31, -10, 10, 16};

const int dx_laser[] = {1, -4, -6, -4, -4, 4, 4, 6, 4, -1};
const int dy_laser[] = {-12, -12, -11, -10, -9, -9, -10, -11, -12, -11};

const int dx_truc_laser[] = {-2, -1, 0, 3, -1, 1, -3, 0, 0, 2};
const int dy_truc_laser[] = {-6, -5, -8, -13, -15, -15, -13, -8, -5, -5};

const int dx_m16_deto[] = {-2, 20, 30, 22, -3, 3, -21, -30, -20, 2};
const int dy_m16_deto[] = {24, 20, -9, -31, -49, -49, -31, -9, 20, 24};

const int dx_m16o_deto[] = {1, 17, 22, 15, -4, 4, -15, -23, -20, -1};
const int dy_m16o_deto[] = {16, 9, -10, -25, -33, -33, -25, -10, 8, 16};

const int dx_pm_deto[] = {-6, 12, 12, 11, -6, 6, -11, -12, -13, -1};
const int dy_pm_deto[] = {6, 5, -11, -22, -28, -30, -22, -11, 5, 6};

const int dx_fusil_deto[] = {2, 29, 41, 29, -5, 5, -29, -41, -30, -2};
const int dy_fusil_deto[] = {33, 21, -12, -44, -57, -57, -44, -12, 21, 33};

const int dx_laser_deto[] = {-3, 24, 37, 29, -1, 0, -29, -37, -24, 2};
const int dy_laser_deto[] = {32, 21, -8, -39, -53, -53, -39, -8, 21, 32};

const int dx_tir_m16[] = {-2, 25, 35, 27, -3, 3, -26, -30, -25, 2};
const int dy_tir_m16[] = {29, 25, -9, -36, -54, -54, -36, -9, 25, 29};

const int dx_tir_pm[] = {-2, 25, 35, 27, -3, 3, -26, -30, -25, 2};
const int dy_tir_pm[] = {29, 25, -9, -36, -54, -54, -36, -9, 25, 29};

const int dx_tir_fusil[] = {2, 29, 41, 29, -5, 5, -29, -41, -30, -2};
const int dy_tir_fusil[] = {33, 21, -12, -44, -57, -57, -44, -12, 21, 33};
const int dx_tir_laser[] = {-5, 42, 57, 56, -3, -2, -56, -57, -42, 0};
const int dy_tir_laser[] = {52, 33, -10, -71, -73, -73, -71, -10, 33, 52};

const int dx_tir_lf[] = {-2, 32, 44, 31, 0, 1, -31, -44, -32, 2};
const int dy_tir_lf[] = {46, 33, -7, -46, -63, -63, -46, -7, 33, 46};

const int x_recul[][5] = {
	{0, 0, 0, 0, 0},	 // BBDIR_BAS_B_D
	{0, -1, -2, -1, 0},	 // BBDIR_BAS_DROITE
	{0, -2, -3, -2, 0},	 // BBDIR_DROITE
	{0, -1, -2, -1, 0},	 // BBDIR_HAUT_DROITE
	{0, 0, 0, 0, 0},	 // BBDIR_HAUT_H_D
	{0, 0, 0, 0, 0},	 // BBDIR_HAUT_H_G
	{0, 1, 2, 1, 0},	 // BBDIR_HAUT_GAUCHE
	{0, 2, 3, 2, 0},	 // BBDIR_GAUCHE
	{0, 1, 2, 1, 0},	 // BBDIR_BAS_GAUCHE
	{0, 0, 0, 0, 0}		 // BBDIR_BAS_B_G
};

const int y_recul[][5] = {
	{0, -2, -3, -2, 0},	 // BBDIR_BAS_B_D
	{0, -1, -2, -1, 0},	 // BBDIR_BAS_DROITE
	{0, 0, 0, 0, 0},	 // BBDIR_DROITE
	{0, 1, 2, 1, 0},	 // BBDIR_HAUT_DROITE
	{0, 2, 3, 2, 0},	 // BBDIR_HAUT_H_D
	{0, 2, 3, 2, 0},	 // BBDIR_HAUT_H_G
	{0, 1, 2, 1, 0},	 // BBDIR_HAUT_GAUCHE
	{0, 0, 0, 0, 0},	 // BBDIR_GAUCHE
	{0, -1, -2, -1, 0},	 // BBDIR_BAS_GAUCHE
	{0, -2, -3, -2, 0}	 // BBDIR_BAS_B_G
};

const int x_recul_fusil[][5] = {
	{0, 0, 0, 0, 0},	 // BBDIR_BAS_B_D
	{0, -2, -4, -2, 0},	 // BBDIR_BAS_DROITE
	{0, -4, -6, -4, 0},	 // BBDIR_DROITE
	{0, -2, -4, -2, 0},	 // BBDIR_HAUT_DROITE
	{0, 0, 0, 0, 0},	 // BBDIR_HAUT_H_D
	{0, 0, 0, 0, 0},	 // BBDIR_HAUT_H_G
	{0, 2, 4, 2, 0},	 // BBDIR_HAUT_GAUCHE
	{0, 4, 6, 4, 0},	 // BBDIR_GAUCHE
	{0, 2, 4, 2, 0},	 // BBDIR_BAS_GAUCHE
	{0, 0, 0, 0, 0}		 // BBDIR_BAS_B_G
};

const int y_recul_fusil[][5] = {
	{0, -4, -6, -4, 0},	 // BBDIR_BAS_B_D
	{0, -2, -4, -2, 0},	 // BBDIR_BAS_DROITE
	{0, 0, 0, 0, 0},	 // BBDIR_DROITE
	{0, 2, 4, 2, 0},	 // BBDIR_HAUT_DROITE
	{0, 4, 6, 4, 0},	 // BBDIR_HAUT_H_D
	{0, 4, 6, 4, 0},	 // BBDIR_HAUT_H_G
	{0, 2, 4, 2, 0},	 // BBDIR_HAUT_GAUCHE
	{0, 0, 0, 0, 0},	 // BBDIR_GAUCHE
	{0, -2, -4, -2, 0},	 // BBDIR_BAS_GAUCHE
	{0, -4, -6, -4, 0}	 // BBDIR_BAS_B_G
};

Ball::Ball(const PictureBank& pbk)
	: sauti(0),
	  ctrl(NULL),
	  id_arme(ID_M16),
	  tire(false),
	  etape_recul(0),
	  dir_arme(0),
	  poid_arme(1),
	  cadence_arme(10),
	  ammo(0),
	  dx_saut(0),
	  latence_arme(3),
	  nb_etape_arme(5),
	  invincible(0),
	  a_mal(0),
	  nb_life(5),
	  nb_cow_bomb(0),
	  time_down(0),
	  wait_cow_bomb(0),
	  dx_glisse(0),
	  latence_glisse(0),
	  perfect(true),
	  next_m16(0),
	  next_pm(0),
	  next_fusil(0),
	  next_laser(0),
	  next_lf(0),
	  locked_fire(false),
	  fire_lf(false),
	  locked_dir(false),
	  etape_cli(0),
	  inv_cow(false),
	  mod_life(0),
	  etape_bouge_arme(0),
	  fire_laser(0),
	  pbk_own(pbk) {
	dir = BBDIR_DROITE;
	col_on = true;
	pv = 5;
}

void Ball::affiche() {
	if (nb_life <= 0) return;

	if (invincible > 50 && etape_cli == 0 && phase && !inv_cow) return;

	switch (etat) {
		case ETAT_NORMAL:
		case ETAT_AVANCE:
			afficheNormal();
			break;

		case ETAT_SALETO:
		case ETAT_COME_BACK:
			Sprite::affiche();
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			afficheSaute();
			break;

		case ETAT_MEURE:
			afficheMeure();
			break;

		case ETAT_LOCKEDV:
			afficheVehicule();
			break;
	}
}

void Ball::afficheNormal() {
	int yback = y;
	y -= anime_sautille[sauti];

	Sprite::affiche();

	afficheOeil(x, y);
	afficheArme(x, y);

	y = yback;
}

// Counterclockwise degrees, the right side spans -90..90 and the left side 90..270
static double angleOf(int d) { return d <= BBLIM_DROITE ? (d - BBDIR_DROITE) * 22.5 : 180 + (d - BBDIR_GAUCHE) * 22.5; }

// Moves a point placed for the hand drawn sprite onto the gun rotated around its grip
static void rotateAround(int& px, int& py, int gx, int gy, double deg) {
	double a = deg * std::numbers::pi / 180, c = std::cos(a), s = std::sin(a);
	int ox = px - gx, oy = py - gy;
	px = gx + (int)std::lround(ox * c + oy * s);
	py = gy + (int)std::lround(-ox * s + oy * c);
}

double Ball::gunAngle() const {
	double a = angleOf(dir_arme);
	// The exact aim goes stale while a vehicle locks the direction
	bool locked = etat == ETAT_LOCKEDV && locked_dir;
	bool precise = (id_arme == ID_M16 || id_arme == ID_PM) && !locked && dir_arme == dir && std::abs(aim_angle_ - a) <= 11.25;
	return precise ? aim_angle_ : a;
}

void Ball::gunSector(int& d_arme, double& residual) const {
	double a = gunAngle();
	bool right = dir_arme <= BBLIM_DROITE;
	// Sprites 0..4 run from down to up on the right, 5..9 from up to down on the left
	int step = std::clamp((int)std::lround(right ? (a + 90) / 45 : (a - 90) / 45), 0, 4);
	d_arme = right ? step : 5 + step;
	double sector = right ? -90 + 45 * step : 90 + 45 * step;
	// The shotgun hand and the laser glow are drawn per sector and do not rotate
	residual = id_arme == ID_LASER || id_arme == ID_FUSIL ? 0 : a - sector;
}

void Ball::afficheArme(int xtmp, int ytmp) {
	// Hand drawn sprites cover 45 degree steps, the rest of the aim rotates them around the grip
	int d_arme;
	double rot;
	gunSector(d_arme, rot);
	int gx = xtmp, gy = ytmp;
	auto put = [&](int px, int py, const Picture* pic) {
		if (rot == 0) return draw(px, py, pic);
		rotateAround(px, py, gx, gy, rot);
		drawRotated(px, py, pic, rot);
	};

	int d_arme2 = d_arme % 5;

	switch (id_arme) {
		case ID_M16:
			gx = xtmp + dx_m16[d_arme];
			gy = ytmp + dy_m16[d_arme];
			if (tire)
				put(gx + x_recul[d_arme][etape_recul], gy + y_recul[d_arme][etape_recul], pbk_own[24 + d_arme]);
			else
				put(gx, gy, pbk_own[24 + d_arme]);
			break;

		case ID_PM:
			gx = xtmp + dx_pm[d_arme];
			gy = ytmp + dy_pm[d_arme];
			if (tire)
				put(gx + x_recul[d_arme][etape_recul], gy + y_recul[d_arme][etape_recul], pbk_own[64 + d_arme]);
			else
				put(gx, gy, pbk_own[64 + d_arme]);
			break;

		case ID_LF:
			gx = xtmp + dx_lf[d_arme];
			gy = ytmp + dy_lf[d_arme];
			put(gx, gy, pbk_own[54 + d_arme]);
			break;

		case ID_FUSIL:
			if (tire) {
				if (etape_arme < 5) {  // Recoil
					draw(xtmp + dx_fusil[d_arme] + x_recul[d_arme][etape_arme] * 2, ytmp + dy_fusil[d_arme] + y_recul[d_arme][etape_arme] * 2,
						 pbk_own[44 + d_arme]);

					draw(xtmp + dx_main_fusil[d_arme] + x_recul_fusil[d_arme][etape_arme], ytmp + dy_main_fusil[d_arme] + y_recul_fusil[d_arme][etape_arme],
						 pbk_own[104 + d_arme2]);
				} else {  // Reload
					int ea = (etape_arme - 5) / 2;

					draw(xtmp + dx_fusil[d_arme], ytmp + dy_fusil[d_arme], pbk_own[44 + d_arme]);
					draw(xtmp + dx_main_fusil[d_arme] + x_recul_fusil[d_arme][ea], ytmp + dy_main_fusil[d_arme] + y_recul_fusil[d_arme][ea],
						 pbk_own[104 + d_arme2]);
				}
			} else {
				draw(xtmp + dx_fusil[d_arme], ytmp + dy_fusil[d_arme], pbk_own[44 + d_arme]);
				draw(xtmp + dx_main_fusil[d_arme], ytmp + dy_main_fusil[d_arme], pbk_own[104 + d_arme2]);
			}
			break;

		case ID_LASER:
			if (tire) {
				draw(xtmp + dx_laser[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_laser[d_arme] + y_recul[d_arme][etape_recul], pbk_own[34 + d_arme]);
				draw(xtmp + dx_truc_laser[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_truc_laser[d_arme] + y_recul[d_arme][etape_recul],
					 pbk_bb[152 + 4 * d_arme + etape_arme]);
			} else {
				draw(xtmp + dx_laser[d_arme], ytmp + dy_laser[d_arme], pbk_own[34 + d_arme]);
			}
			break;
	}

	if (tire) {
		int d_arme2 = d_arme;
		if (d_arme == 9)
			d_arme2 = 0;
		else if (d_arme > 4)
			d_arme2 -= 1;

		switch (id_arme) {
			case ID_M16:
				if (id_ball == ID_BLIP) {
					put(xtmp + dx_m16_deto[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_m16_deto[d_arme] + y_recul[d_arme][etape_recul],
						pbk_bb[12 + d_arme * 3 + anim_m16_deto[etape_arme]]);
				} else {
					put(xtmp + dx_m16o_deto[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_m16o_deto[d_arme] + y_recul[d_arme][etape_recul],
						pbk_bb[12 + d_arme * 3 + anim_m16_deto[etape_arme]]);
				}
				break;

			case ID_PM:
				put(xtmp + dx_pm_deto[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_pm_deto[d_arme] + y_recul[d_arme][etape_recul],
					pbk_own[74 + d_arme * 3 + anim_pm_deto[etape_arme]]);
				break;

			case ID_FUSIL:
				if (etape_arme < 5)
					draw(xtmp + dx_fusil_deto[d_arme] + x_recul_fusil[d_arme][etape_arme], ytmp + dy_fusil_deto[d_arme] + y_recul_fusil[d_arme][etape_arme],
						 pbk_bb[42 + d_arme * 3 + anim_fusil_deto[etape_arme]]);
				break;

			case ID_LASER:
				if (dir_arme == dir)
					draw(xtmp + dx_laser_deto[d_arme] + x_recul[d_arme][etape_recul], ytmp + dy_laser_deto[d_arme] + y_recul[d_arme][etape_recul],
						 pbk_bb[104 + d_arme2 * 4 + etape_arme]);
				break;
		}
	}
}

void Ball::afficheOeil(int xtmp, int ytmp) {
	int base;
	int delta;

	if (a_mal) {
		if (dir <= BBLIM_DROITE)
			draw(xtmp + 12, ytmp - 14, pbk_own[22]);
		else
			draw(xtmp - 12, ytmp - 14, pbk_own[23]);
	} else {
		if (tire)
			base = 6;
		else
			base = 0;

		if (dir <= BBDIR_BAS_DROITE)
			delta = 0;
		else if (dir <= BBDIR_DROITE)
			delta = 1;
		else if (dir <= BBDIR_HAUT_H_D)
			delta = 2;
		else if (dir <= BBDIR_HAUT_GAUCHE)
			delta = 3;
		else if (dir <= BBDIR_GAUCHE)
			delta = 4;
		else
			delta = 5;

		if (dir <= BBLIM_DROITE)
			draw(xtmp + 12, ytmp - 14, pbk_bb[base + delta]);
		else
			draw(xtmp - 12, ytmp - 14, pbk_bb[base + delta]);
	}
}

void Ball::afficheSaute() {
	Sprite::affiche();

	if (dy < 0) {
		afficheOeil(x, y + dy - 2);
		afficheArme(x, y + dy - 2);
	} else {
		afficheOeil(x, y);
		afficheArme(x, y);
	}
}

void Ball::update() {
	if (phase) {
		etape_cli += 1;
		etape_cli %= 4;
	}

	if (id_arme == ID_LF) {
		if (tire && !fire_lf) {
			sbk_bb.play(2, SOUND_LOOP);
			fire_lf = true;
		} else if (!tire && fire_lf) {
			sbk_bb.stop(2);
			fire_lf = false;
		}
	} else if (fire_lf) {
		sbk_bb.stop(2);
		fire_lf = false;
	}

	if (id_arme == ID_LASER) {
		if (tire && !fire_laser) {
			sbk_bb.play(4, SOUND_LOOP);
			fire_laser = true;
		} else if (!tire && fire_laser) {
			sbk_bb.stop(4);
			fire_laser = false;
		}
	} else if (fire_laser) {
		sbk_bb.stop(4);
		fire_laser = false;
	}

	if (nb_life <= 0) return;

	if (y > 540) {
		nb_life = 0;
		perfect = false;
		a_detruire = true;
	}

	if (etat == ETAT_MEURE) {
		onMeure();
		return;
	}

	if (etat == ETAT_COME_BACK) {
		onComeBack();
		return;
	}

	if (wait_cow_bomb < 100) wait_cow_bomb += 1;

	if (ctrl->super() && wait_cow_bomb == 100 && nb_cow_bomb >= 1) {
		ShotBBCow* ptr;

		for (int i = 0; i < 11; i++) {
			int xVache = offset + i * 64 + rand() % 20;

			if (!mur_opaque(xVache, 0)) {
				ptr = new ShotBBCow();

				ptr->x = xVache;
				ptr->y = rand() % 180 - 200;
				ptr->dy = 5 + rand() % 3;
				ptr->etape = rand() % 8;
				ptr->joueur = this;

				list_cow.push_back(std::unique_ptr<ShotBBCow>(ptr));
			}
		}

		wait_cow_bomb = 0;
		nb_cow_bomb -= 1;
		invincible = 200;
		inv_cow = true;

		sbk_bb.play(5);
	}

	// The scroll pushes players along the left edge
	if (x < offset + 20 && !ctrl->droite() && !mur_opaque(x + 2, y)) {
		x += 2;

		if (latence_glisse < 0) dx_glisse = latence_glisse = 0;
	}

	int yp = plat(x, y);

	if (yp != 0) y = yp;

	// A player pinned against a wall by the scroll dies
	if (x < offset - 30) {
		no_scroll1 = true;
		nb_life -= 1;

		if (nb_life == 0) {
			a_detruire = true;
			return;
		}

		etat = ETAT_COME_BACK;

		x = offset + 320;
		int i = 0;

		while (i < NB_MAX_PLAT && y_plat[i][x] == 0xFFFF) i++;

		y_to_go = i < NB_MAX_PLAT ? y_plat[i][x] : 0xFFFF;

		y = -50;
		col_on = true;
		pv = 5;
		dir_arme = dir = BBDIR_DROITE;
		invincible = 400;
		id_arme = ID_M16;
		latence_arme = 3;
		nb_etape_arme = 5;
		cadence_arme = 10;
		poid_arme = 1;
		ammo = 0;
		pic = pbk_own[8];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_saut = 0;
		tire = false;
	}

	if (invincible > 0) {
		invincible -= 1;
	} else {
		inv_cow = false;
	}

	if (a_mal > 0) a_mal -= 1;

	if (ss_etape == 0) {
		sauti++;
		sauti %= 4;
	}

	// Detect a quick double tap on down, which drops through the platform
	if (ctrl->bas()) {
		if (time_down > 0 && time_down < 10 && etat == ETAT_NORMAL)
			time_down = -1;
		else
			time_down = 0;
	} else {
		time_down += 1;
	}

	if (etat != ETAT_LOCKEDV || !locked_dir) manageDirection();

	switch (etat) {
		case ETAT_NORMAL:
			onNormal();
			break;

		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_SALETO:
			onSaleto();
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onSaute();
			break;

		case ETAT_LOCKEDV:
			onVehicule();
			break;
	}

	// Ice keeps some momentum
	if (etat == ETAT_NORMAL || etat == ETAT_AVANCE) {
		int nplat = plat2(x, y);

		if (nplat >= 6 && x > 10 && x > offset + 30 && x < offset + 610) {
			int ytmp1 = y_plat[nplat][x + 10] - y;
			int ytmp2 = y_plat[nplat][x - 10] - y;

			if (ytmp1 < 10 && ytmp1 > -10) latence_glisse += ytmp1;
			if (ytmp2 < 10 && ytmp2 > -10) latence_glisse -= ytmp2;

			if (latence_glisse > 0) {
				if (ctrl->droite())
					latence_glisse += 25;
				else if (ctrl->gauche())
					latence_glisse -= 10;
				else
					latence_glisse -= 1;
			} else if (latence_glisse < 0) {
				if (ctrl->gauche())
					latence_glisse -= 25;
				else if (ctrl->droite())
					latence_glisse += 10;
				else
					latence_glisse += 1;
			} else {
				if (ctrl->droite())
					latence_glisse += 25;
				else if (ctrl->gauche())
					latence_glisse -= 25;
			}

			if (latence_glisse > 600)
				latence_glisse = 600;
			else if (latence_glisse < -600)
				latence_glisse = -600;

			if (latence_glisse > 0)
				dx_glisse = latence_glisse / 200 + 1;
			else if (latence_glisse < 0)
				dx_glisse = latence_glisse / 200 - 1;
			else
				dx_glisse = 0;
		} else {
			dx_glisse = latence_glisse = 0;
		}
	} else {
		dx_glisse = latence_glisse = 0;
	}

	colFromPic();

	if (etat != ETAT_SALETO) updateArme();

	return;
}

void Ball::onNormal() {
	if (time_down == -1 && !mur_opaque(x, y + 5) && !mur_opaque(x, y + 10) && !mur_opaque(x, y + 15) && !mur_opaque(x, y + 20) && !mur_opaque(x, y + 25)) {
		y += 10;
		dy = 5;
		etat = ETAT_TOMBE;
		lat_grav = 0;
		dx_saut = dx_glisse;
		time_down = 0;
		onSaute();
		return;
	}

	if (ctrl->saut() && !mur_opaque(x, y - 5)) {
		etat = ETAT_SAUTE;
		dy = -5;
		lat_grav = 0;  // Otherwise jump heights vary

		dx_saut = dx_glisse;

		if (ctrl->gauche() && dx_saut > -2)
			dx_saut = -2;
		else if (ctrl->droite() && dx_saut < 2)
			dx_saut = 2;

		jump_released = false;
		onSaute();
		return;
	}

	if (ctrl->gauche() || ctrl->droite() || dx_glisse != 0) {
		etat = ETAT_AVANCE;
		onAvance();
		return;
	}

	if (plat(x, y) == 0) {
		etat = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		dx_saut = dx_glisse;

		if (ctrl->gauche() && dx_saut > -2)
			dx_saut = -2;
		else if (ctrl->droite() && dx_saut < 2)
			dx_saut = 2;

		return;
	}

	if (dir <= BBLIM_DROITE)
		pic = pbk_own[anime(anim_blip_marche_droite, 4, 8)];
	else
		pic = pbk_own[anime(anim_blip_marche_gauche, 4, 8)];
}

void Ball::onAvance() {
	if (ctrl->saut() && !mur_opaque(x, y - 5)) {
		etat = ETAT_SAUTE;
		dy = -5;
		lat_grav = 0;  // Otherwise jump heights vary

		dx_saut = dx_glisse;

		if (ctrl->gauche() && dx_saut > -2)
			dx_saut = -2;
		else if (ctrl->droite() && dx_saut < 2)
			dx_saut = 2;

		jump_released = false;
		onSaute();
		return;
	}

	int destx = x + dx_glisse;

	if ((dx_glisse > 0 && !mur_opaque(destx, y) && destx < offset + 620) || (dx_glisse < 0 && !mur_opaque(destx, y) && destx > offset + 20))
		marche(dx_glisse);
	else {
		if (ctrl->droite() && (!mur_opaque(x + 2, y) && x + 2 < offset + 620)) {
			marche(2);
		} else if (ctrl->gauche() && (!mur_opaque(x - 2, y) && x - 2 > offset + 20)) {
			marche(-2);
		} else {
			etat = ETAT_NORMAL;
		}
	}

	if (plat(x, y) == 0) {
		etat = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		dx_saut = dx_glisse;

		if (ctrl->gauche() && dx_saut > -2)
			dx_saut = -2;
		else if (ctrl->droite() && dx_saut < 2)
			dx_saut = 2;

		onSaute();
		return;
	}

	if (dir <= BBLIM_DROITE)
		pic = pbk_own[anime(anim_blip_marche_droite, 4, 5)];
	else
		pic = pbk_own[anime(anim_blip_marche_gauche, 4, 5)];
}

void Ball::onSaute() {
	int yp;

	// A somersault needs jump released and pressed again mid jump
	if (!ctrl->saut()) jump_released = true;

	if (ctrl->saut() && jump_released && etat == ETAT_SAUTE && dy < 3) {
		dy = -5;
		etat = ETAT_SALETO;
		etape = 0;
		ss_etape = 0;
		tire = false;
		etape_arme = ss_etape_arme = 0;
		onSaleto();
		return;
	}

	tombe2();

	// Releasing jump early makes a shorter jump
	if (dy < 0 && !ctrl->saut()) {
		lat_grav += 1;
		lat_grav %= LATENCE_GRAVITE;

		if (lat_grav == 0) {
			dy += 1;
		}
	}

	if (dy > 0 && (yp = plat(x, y + dy)) != 0) {
		etat = ETAT_NORMAL;
		latence_glisse = 200 * dx_saut;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (!ctrl->inertia() || (!ctrl->bas() && !ctrl->haut())) {
		if (ctrl->droite() && dx_saut < 2)
			dx_saut = 2;
		else if (ctrl->gauche() && dx_saut > -2)
			dx_saut = -2;
		else if (!ctrl->inertia())
			dx_saut = 0;
	}

	int xx = x + dx_saut;

	if (!mur_opaque(xx, y) && xx > offset + 20 && xx < offset + 620) x = xx;

	if (dir <= BBLIM_DROITE)
		pic = pbk_own[anim_blip_saute_droite[dy + 5]];
	else
		pic = pbk_own[anim_blip_saute_gauche[dy + 5]];
}

void Ball::manageDirection() {
	// The pivot is the gun height, the feet are at y. No aim returns to horizontal
	int vx, vy;
	ctrl->aim(x - offset, y - 12, vx, vy);
	double t = std::atan2(-vy, std::abs(vx));
	int s = (int)std::lround(t / (std::numbers::pi / 8));
	// Laser beams and shotgun spreads only exist in 8 directions
	if (id_arme == ID_LASER || id_arme == ID_FUSIL) s = 2 * (int)std::lround(s / 2.0);
	bool right = dir <= BBLIM_DROITE;
	// Straight up or down keeps the facing, so the body does not flip while aiming there
	if (std::abs(s) < 4 && vx != 0) right = vx > 0;
	dir = right ? BBDIR_DROITE + s : BBDIR_GAUCHE - s;
	double deg = t * 180 / std::numbers::pi;
	// Mirrored by the aim itself, then kept in the facing's range so near vertical aim stays on its side
	aim_angle_ = vx < 0 ? 180 - deg : deg;
	if (right && aim_angle_ > 180) aim_angle_ -= 360;
	if (!right && aim_angle_ < 0) aim_angle_ += 360;

	if (dir <= BBLIM_DROITE && dir_arme > BBLIM_DROITE)
		dir_arme = BBDIR_BAS_B_G - dir_arme;
	else if (dir > BBLIM_DROITE && dir_arme <= BBLIM_DROITE)
		dir_arme = 17 - dir_arme;

	etape_bouge_arme += 1;
	etape_bouge_arme %= poid_arme;

	if (etape_bouge_arme == 0) {
		if (dir_arme < dir)
			dir_arme += 1;
		else if (dir_arme > dir)
			dir_arme -= 1;
	}
}

void Ball::updateArme() {
	if (etat == ETAT_LOCKEDV) return;

	int yback = y;

	switch (etat) {
		case ETAT_NORMAL:
		case ETAT_AVANCE:
			y -= anime_sautille[sauti];
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			if (dy < 0) y += dy - 2;
			break;
	}

	if (tire) {
		ss_etape_arme += 1;
		ss_etape_arme %= latence_arme;

		if (ss_etape_arme == 0) etape_arme += 1;

		if (etape_arme == nb_etape_arme) tire = false;
	}

	if (!tire && ctrl->fire() && !hold_fire) {
		tire = true;
		etape_arme = 0;
		ss_etape_arme = 0;

		switch (id_arme) {
			case ID_M16:
				break;

			case ID_PM:
				sbk_bb.play(0);
				break;

			case ID_LF:
				break;

			case ID_FUSIL:
				break;

			case ID_LASER:
				break;
		}

		pruno = cadence_arme - 1;
	}

	etape_recul += 1;
	etape_recul %= 5;

	pruno += 1;
	pruno %= cadence_arme;

	if (tire && pruno == 0 && !(poid_arme <= 1 && dir_arme != dir)) {
		int d_arme;
		double rot;
		gunSector(d_arme, rot);
		// Muzzle of the rotated gun, from the sprite's grip
		auto muzzle = [&](ShotBB* t, const int* gdx, const int* gdy, int mx, int my) {
			t->x = x + mx;
			t->y = y + my;
			rotateAround(t->x, t->y, x + gdx[d_arme], y + gdy[d_arme], rot);
		};

		ShotBB* t = NULL;
		int i;

		switch (id_arme) {
			case ID_M16:

				sbk_bb.play(3);

				next_m16 = (next_m16 + 1) % NB_TIRS_M16;
				t = &tirs_m16[next_m16];
				t->a_detruire = false;

				t->joueur = this;
				muzzle(t, dx_m16, dy_m16, dx_tir_m16[d_arme] + rand() % 11 - 5, dy_tir_m16[d_arme] + rand() % 11 - 5);

				if (etat == ETAT_LOCKEDV)
					t->dxReferentiel = scroll_speed;
				else
					t->dxReferentiel = 0;

				t->setAngle(gunAngle(), 9);
				list_tirs_bb.push_back(t);
				break;

			case ID_PM:

				next_pm = (next_pm + 1) % NB_TIRS_PM;
				t = &tirs_pm[next_pm];
				t->a_detruire = false;

				t->joueur = this;
				muzzle(t, dx_pm, dy_pm, dx_tir_pm[d_arme] + rand() % 11 - 5, dy_tir_pm[d_arme] + rand() % 11 - 5);

				if (etat == ETAT_LOCKEDV)
					t->dxReferentiel = scroll_speed;
				else
					t->dxReferentiel = 0;

				t->setAngle(gunAngle(), 12);
				list_tirs_bb.push_back(t);
				break;

			case ID_FUSIL:

				sbk_bb.play(1);

				for (i = 0; i < 17; i++) {
					next_fusil = (next_fusil + 1) % NB_TIRS_FUSIL;
					t = &tirs_fusil[next_fusil];
					t->a_detruire = false;

					t->joueur = this;
					t->x = x + dx_tir_fusil[d_arme] + rand() % 11 - 5;
					t->y = y + dy_tir_fusil[d_arme] + rand() % 11 - 5;

					if (etat == ETAT_LOCKEDV)
						t->dxReferentiel = scroll_speed;
					else
						t->dxReferentiel = 0;

					t->setDir(dir);
					list_tirs_bb.push_back(t);
				}
				break;

			case ID_LASER:

				next_laser = (next_laser + 1) % NB_TIRS_LASER;
				t = &tirs_laser[next_laser];
				t->a_detruire = false;

				t->joueur = this;
				t->x = x + dx_tir_laser[d_arme] + x_recul[d_arme][etape_recul];
				t->y = y + dy_tir_laser[d_arme] + y_recul[d_arme][etape_recul];
				t->setDir(dir);
				t->etape = etape_arme;
				list_tirs_bb.push_back(t);
				break;

			case ID_LF:
				next_lf = (next_lf + 1) % NB_TIRS_LF;
				t = &tirs_lf[next_lf];
				t->a_detruire = false;

				((ShotBBLF*)t)->deg = 2;
				t->joueur = this;
				muzzle(t, dx_lf, dy_lf, dx_tir_lf[d_arme] + x_recul[d_arme][etape_recul], dy_tir_lf[d_arme] + y_recul[d_arme][etape_recul]);
				t->setDir(dir_arme);

				list_tirs_bb.push_back(t);

				next_lf = (next_lf + 1) % NB_TIRS_LF;
				t = &tirs_lf[next_lf];
				t->a_detruire = false;

				((ShotBBLF*)t)->deg = 1;
				t->joueur = this;
				muzzle(t, dx_lf, dy_lf, dx_tir_lf[d_arme] + x_recul[d_arme][etape_recul], dy_tir_lf[d_arme] + y_recul[d_arme][etape_recul]);
				t->setDir(dir_arme);

				list_tirs_bb.push_back(t);
				break;
		}

		if (id_arme != ID_M16) {
			ammo -= 1;

			if (ammo <= 0) {
				id_arme = ID_M16;
				latence_arme = 3;
				nb_etape_arme = 5;
				cadence_arme = 10;
				poid_arme = 1;
				tire = false;
			}
		}
	}

	y = yback;
}

void Ball::estTouche(const Shot* tir) {}

void Ball::estTouche(int degats) {
	if (invincible > 0 || etat == ETAT_MEURE) return;

	pv -= degats;
	perfect = false;
	if (pv <= 0) {
		pv = 0;
		etat = ETAT_MEURE;
		tire = false;
		etape_arme = ss_etape_arme = 0;
		etape = 0;
		ss_etape = 0;
		col_on = false;
		no_scroll1 = true;
	} else {
		invincible = 200;
		a_mal = 100;
	}
}

void Ball::onMeure() {
	if (dy < 0) dy = 0;

	tombe();

	ss_etape += 1;
	ss_etape %= 7;

	if (ss_etape == 0) etape += 1;

	if (etape == 15) {
		nb_life -= 1;

		if (nb_life == 0) {
			a_detruire = true;
			no_scroll1 = false;
			return;
		}

		etat = ETAT_COME_BACK;
		y_to_go = y;
		y = -50;
		col_on = true;
		pv = 5;
		dir_arme = dir = BBDIR_DROITE;
		invincible = 400;

		rearme();

		pic = pbk_own[8];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_saut = 0;
		tire = false;

		if (cowBombOn) nb_cow_bomb = 1;
	} else if (etape == 5 && ss_etape == 0 && plat(x, y) != 0) {
		if (dir <= BBLIM_DROITE)
			grave(x, y, pbk_own[16]);
		else
			grave(x, y, pbk_own[21]);
	} else if (etape < 5) {
		if (dir <= BBLIM_DROITE)
			pic = pbk_own[12 + etape];
		else
			pic = pbk_own[17 + etape];
	}
}

void Ball::afficheMeure() {
	if (etape < 5) Sprite::affiche();
}

void Ball::onComeBack() {
	no_scroll1 = true;

	if (y < y_to_go) {
		ss_etape += 1;
		ss_etape %= 3;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 8;
		}

		y += 5;

		pic = pbk_own[109 + id_arme * 8 + etape];
	} else {
		y = y_to_go;
		etat = ETAT_NORMAL;
		etape = 0;
		ss_etape = 0;
		no_scroll1 = false;
	}
}

void Ball::onSaleto() {
	int yp;

	ss_etape += 1;
	ss_etape %= 3;

	if (ss_etape == 0) {
		etape += 1;
		etape %= 8;
	}

	tombe2();

	if (dy > 0 && (yp = plat(x, y + dy)) != 0) {
		etat = ETAT_NORMAL;
		latence_glisse = 200 * dx_saut;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (ctrl->droite() && dx_saut < 2)
		dx_saut = 2;
	else if (ctrl->gauche() && dx_saut > -2)
		dx_saut = -2;

	int xx = x + dx_saut;

	if (!mur_opaque(xx, y) && xx > offset + 20 && xx < offset + 620) x = xx;

	pic = pbk_own[109 + id_arme * 8 + etape];
}

bool Ball::okBonus() {
	return (id_arme == ID_M16 || (id_arme == ID_PM && ammo <= 30) || (id_arme == ID_FUSIL && ammo <= 4) || (id_arme == ID_LF && ammo <= 100) ||
			(id_arme == ID_LASER && ammo <= 100));
}

void Ball::setSuperWeapon() {
	if (id_ball == ID_BLIP || !okLanceFlame) {
		id_arme = ID_LASER;
		ammo = 750;
		latence_arme = 3;
		nb_etape_arme = 4;
		cadence_arme = 1;
		poid_arme = 1;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		tire = false;
	} else {
		id_arme = ID_LF;
		ammo = 1000;
		latence_arme = 3;
		nb_etape_arme = 5;
		cadence_arme = 1;
		poid_arme = 5;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		tire = false;
	}
}

void Ball::colFromPic() {
	Sprite::colFromPic();

	if (etat == ETAT_SALETO) {
		x1 += 15;
		x2 -= 15;
		y1 += 15;
		y2 -= 15;
	} else {
		x1 += 10;
		x2 -= 10;
		y1 += 10;
		y2 -= 10;
	}
}

void Ball::lockVehicule(bool can_fire, bool can_dir) {
	etat = ETAT_LOCKEDV;
	locked_fire = !can_fire;
	locked_dir = !can_dir;

	if (!can_fire) {
		tire = false;
		etape_arme = ss_etape_arme = 0;
	}
}

void Ball::unlockVehicule() {
	etape = ss_etape = 0;
	etat = ETAT_SALETO;
	dy = -5;
	dx_saut = 0;
	tire = false;
	etape_arme = ss_etape_arme = 0;
}

void Ball::onVehicule() {}

void Ball::afficheVehicule() {
	if (dir <= BBLIM_DROITE)
		pic = pbk_own[0];
	else
		pic = pbk_own[3];

	Sprite::affiche();
	afficheOeil(x, y);

	afficheArme(x, y);
}
