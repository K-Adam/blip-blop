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

const int anim_walk[] = {0, 1, 2, 1};
const int anim_jump[] = {2, 2, 2, 1, 1, 0, 0, 3, 3, 3, 3};

const int hop_offsets[] = {0, 2, 4, 2};

const int anim_m16_flash[] = {0, 1, 2, 1, 0};
const int anim_pm_flash[] = {0, 1, 2, 1, 0};
const int anim_shotgun_flash[] = {0, 1, 2, 1, 0};

const int dx_m16[] = {3, 6, 0, 2, 7, -7, -2, 0, -6, -3};
const int dy_m16[] = {-15, -9, -12, -10, -10, -10, -10, -12, -9, -15};

const int dx_pm[] = {2, 6, 0, 0, 7, -7, 0, 0, -6, -2};
const int dy_pm[] = {-12, -6, -10, -15, -9, -9, -15, -10, -6, -12};

const int dx_lf[] = {-10, -10, -10, -10, -10, 10, 10, 10, 10, 10};
const int dy_lf[] = {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10};

const int dx_shotgun[] = {1, -4, -6, -4, -4, 4, 4, 6, 4, -1};
const int dy_shotgun[] = {-14, -12, -11, -10, -9, -9, -10, -11, -12, -14};

const int dx_hand_shotgun[] = {0, 17, 24, 18, -3, 3, -18, -24, -17, 0};
const int dy_hand_shotgun[] = {16, 10, -10, -31, -40, -38, -31, -10, 10, 16};

const int dx_laser[] = {1, -4, -6, -4, -4, 4, 4, 6, 4, -1};
const int dy_laser[] = {-12, -12, -11, -10, -9, -9, -10, -11, -12, -11};

const int dx_laser_part[] = {-2, -1, 0, 3, -1, 1, -3, 0, 0, 2};
const int dy_laser_part[] = {-6, -5, -8, -13, -15, -15, -13, -8, -5, -5};

const int dx_m16_flash[] = {-2, 20, 30, 22, -3, 3, -21, -30, -20, 2};
const int dy_m16_flash[] = {24, 20, -9, -31, -49, -49, -31, -9, 20, 24};

const int dx_m16o_flash[] = {1, 17, 22, 15, -4, 4, -15, -23, -20, -1};
const int dy_m16o_flash[] = {16, 9, -10, -25, -33, -33, -25, -10, 8, 16};

const int dx_pm_flash[] = {-6, 12, 12, 11, -6, 6, -11, -12, -13, -1};
const int dy_pm_flash[] = {6, 5, -11, -22, -28, -30, -22, -11, 5, 6};

const int dx_shotgun_flash[] = {2, 29, 41, 29, -5, 5, -29, -41, -30, -2};
const int dy_shotgun_flash[] = {33, 21, -12, -44, -57, -57, -44, -12, 21, 33};

const int dx_laser_flash[] = {-3, 24, 37, 29, -1, 0, -29, -37, -24, 2};
const int dy_laser_flash[] = {32, 21, -8, -39, -53, -53, -39, -8, 21, 32};

const int dx_shot_m16[] = {-2, 25, 35, 27, -3, 3, -26, -30, -25, 2};
const int dy_shot_m16[] = {29, 25, -9, -36, -54, -54, -36, -9, 25, 29};

const int dx_shot_pm[] = {-2, 25, 35, 27, -3, 3, -26, -30, -25, 2};
const int dy_shot_pm[] = {29, 25, -9, -36, -54, -54, -36, -9, 25, 29};

const int dx_shot_shotgun[] = {2, 29, 41, 29, -5, 5, -29, -41, -30, -2};
const int dy_shot_shotgun[] = {33, 21, -12, -44, -57, -57, -44, -12, 21, 33};
const int dx_shot_laser[] = {-2, 42, 57, 56, 0, 1, -56, -57, -42, 3};
const int dy_shot_laser[] = {52, 33, -10, -71, -73, -73, -71, -10, 33, 52};

const int dx_shot_lf[] = {-2, 32, 44, 31, 0, 1, -31, -44, -32, 2};
const int dy_shot_lf[] = {46, 33, -7, -46, -63, -63, -46, -7, 33, 46};

const int x_recoil[][5] = {
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

const int y_recoil[][5] = {
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

const int x_recoil_shotgun[][5] = {
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

const int y_recoil_shotgun[][5] = {
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
	: hop_idx(0),
	  ctrl(NULL),
	  weapon_id(ID_M16),
	  is_firing(false),
	  recoil_step(0),
	  weapon_dir(0),
	  gun_turn_delay(1),
	  fire_interval(10),
	  ammo(0),
	  dx_jump(0),
	  weapon_anim_delay(3),
	  nb_weapon_steps(5),
	  invincible(0),
	  hurt_timer(0),
	  nb_life(5),
	  nb_cow_bomb(0),
	  time_down(0),
	  wait_cow_bomb(0),
	  dx_slide(0),
	  slide_delay(0),
	  perfect(true),
	  next_m16(0),
	  next_pm(0),
	  next_shotgun(0),
	  next_laser(0),
	  next_lf(0),
	  locked_fire(false),
	  fire_lf(false),
	  locked_dir(false),
	  blink_step(0),
	  inv_cow(false),
	  mod_life(0),
	  gun_turn_tick(0),
	  fire_laser(0),
	  pbk_own(pbk) {
	dir = BBDIR_RIGHT;
	col_on = true;
	hp = 5;
}

void Ball::render() {
	if (nb_life <= 0) return;

	if (invincible > 50 && blink_step == 0 && phase && !inv_cow) return;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			renderNormal();
			break;

		case STATE_SOMERSAULT:
		case STATE_COME_BACK:
			Sprite::render();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			renderJump();
			break;

		case STATE_DYING:
			renderDying();
			break;

		case STATE_LOCKED_VEHICLE:
			renderVehicle();
			break;
	}
}

void Ball::renderNormal() {
	int yback = y;
	y -= hop_offsets[hop_idx];

	Sprite::render();

	renderEye(x, y);
	renderWeapon(x, y);

	y = yback;
}

// Counterclockwise degrees, the right side spans -90..90 and the left side 90..270
static double angleOf(int d) { return d <= BBLIM_RIGHT ? (d - BBDIR_RIGHT) * 22.5 : 180 + (d - BBDIR_LEFT) * 22.5; }

double Ball::gunAngle() const {
	double a = angleOf(weapon_dir);
	// The exact aim goes stale while a vehicle locks the direction
	bool locked = status == STATE_LOCKED_VEHICLE && locked_dir;
	bool precise = !locked && weapon_dir == dir && std::abs(aim_angle_ - a) <= 11.25;
	return precise ? aim_angle_ : a;
}

void Ball::gunSector(int& gun_sector, double& residual) const {
	double a = gunAngle();
	bool right = weapon_dir <= BBLIM_RIGHT;
	// Sprites 0..4 run from down to up on the right, 5..9 from up to down on the left
	int step = std::clamp((int)std::lround(right ? (a + 90) / 45 : (a - 90) / 45), 0, 4);
	gun_sector = right ? step : 5 + step;
	double sector = right ? -90 + 45 * step : 90 + 45 * step;
	residual = a - sector;
}

void Ball::renderWeapon(int xtmp, int ytmp) {
	// Hand drawn sprites cover 45 degree steps, the rest of the aim rotates them around the grip
	int gun_sector;
	double rot;
	gunSector(gun_sector, rot);
	int gx = xtmp, gy = ytmp;
	auto put = [&](int px, int py, const Picture* pic) {
		if (rot == 0) return draw(px, py, pic);
		rotateAround(px, py, gx, gy, rot);
		drawRotated(px, py, pic, rot);
	};
	// Shared effects come drawn pointing down only
	double turn = gunAngle() + 90;
	auto putTurned = [&](int px, int py, const Picture* pic) {
		rotateAround(px, py, gx, gy, rot);
		drawRotated(px, py, pic, turn);
	};

	int gun_sector2 = gun_sector % 5;

	switch (weapon_id) {
		case ID_M16:
			gx = xtmp + dx_m16[gun_sector];
			gy = ytmp + dy_m16[gun_sector];
			if (is_firing)
				put(gx + x_recoil[gun_sector][recoil_step], gy + y_recoil[gun_sector][recoil_step], pbk_own.anim("gun_m16")[gun_sector]);
			else
				put(gx, gy, pbk_own.anim("gun_m16")[gun_sector]);
			break;

		case ID_PM:
			gx = xtmp + dx_pm[gun_sector];
			gy = ytmp + dy_pm[gun_sector];
			if (is_firing)
				put(gx + x_recoil[gun_sector][recoil_step], gy + y_recoil[gun_sector][recoil_step], pbk_own.anim("gun_pm")[gun_sector]);
			else
				put(gx, gy, pbk_own.anim("gun_pm")[gun_sector]);
			break;

		case ID_LF:
			gx = xtmp + dx_lf[gun_sector];
			gy = ytmp + dy_lf[gun_sector];
			put(gx, gy, pbk_own.anim("gun_lf")[gun_sector]);
			break;

		case ID_SHOTGUN:
			gx = xtmp + dx_shotgun[gun_sector];
			gy = ytmp + dy_shotgun[gun_sector];
			if (is_firing) {
				if (weapon_step < 5) {	// Recoil
					put(gx + x_recoil[gun_sector][weapon_step] * 2, gy + y_recoil[gun_sector][weapon_step] * 2, pbk_own.anim("gun_shotgun")[gun_sector]);

					put(xtmp + dx_hand_shotgun[gun_sector] + x_recoil_shotgun[gun_sector][weapon_step],
						ytmp + dy_hand_shotgun[gun_sector] + y_recoil_shotgun[gun_sector][weapon_step], pbk_own.anim("shotgun_hand")[gun_sector2]);
				} else {  // Reload
					int ea = (weapon_step - 5) / 2;

					put(gx, gy, pbk_own.anim("gun_shotgun")[gun_sector]);
					put(xtmp + dx_hand_shotgun[gun_sector] + x_recoil_shotgun[gun_sector][ea],
						ytmp + dy_hand_shotgun[gun_sector] + y_recoil_shotgun[gun_sector][ea], pbk_own.anim("shotgun_hand")[gun_sector2]);
				}
			} else {
				put(gx, gy, pbk_own.anim("gun_shotgun")[gun_sector]);
				put(xtmp + dx_hand_shotgun[gun_sector], ytmp + dy_hand_shotgun[gun_sector], pbk_own.anim("shotgun_hand")[gun_sector2]);
			}
			break;

		case ID_LASER:
			gx = xtmp + dx_laser[gun_sector];
			gy = ytmp + dy_laser[gun_sector];
			if (is_firing) {
				put(gx + x_recoil[gun_sector][recoil_step], gy + y_recoil[gun_sector][recoil_step], pbk_own.anim("gun_laser")[gun_sector]);
				putTurned(xtmp + dx_laser_part[gun_sector] + x_recoil[gun_sector][recoil_step],
						  ytmp + dy_laser_part[gun_sector] + y_recoil[gun_sector][recoil_step], pbk_bb.anim("laser_glow")[weapon_step]);
			} else {
				put(gx, gy, pbk_own.anim("gun_laser")[gun_sector]);
			}
			break;
	}

	if (is_firing) {
		switch (weapon_id) {
			case ID_M16:
				if (id_ball == ID_BLIP) {
					putTurned(xtmp + dx_m16_flash[gun_sector] + x_recoil[gun_sector][recoil_step],
							  ytmp + dy_m16_flash[gun_sector] + y_recoil[gun_sector][recoil_step], pbk_bb.anim("flash_m16")[anim_m16_flash[weapon_step]]);
				} else {
					putTurned(xtmp + dx_m16o_flash[gun_sector] + x_recoil[gun_sector][recoil_step],
							  ytmp + dy_m16o_flash[gun_sector] + y_recoil[gun_sector][recoil_step], pbk_bb.anim("flash_m16")[anim_m16_flash[weapon_step]]);
				}
				break;

			case ID_PM:
				put(xtmp + dx_pm_flash[gun_sector] + x_recoil[gun_sector][recoil_step], ytmp + dy_pm_flash[gun_sector] + y_recoil[gun_sector][recoil_step],
					pbk_own.anim("flash_pm")[gun_sector * 3 + anim_pm_flash[weapon_step]]);
				break;

			case ID_SHOTGUN:
				if (weapon_step < 5)
					putTurned(xtmp + dx_shotgun_flash[gun_sector] + x_recoil_shotgun[gun_sector][weapon_step],
							  ytmp + dy_shotgun_flash[gun_sector] + y_recoil_shotgun[gun_sector][weapon_step],
							  pbk_bb.anim("flash_shotgun")[anim_shotgun_flash[weapon_step]]);
				break;

			case ID_LASER:
				if (weapon_dir == dir)
					putTurned(xtmp + dx_laser_flash[gun_sector] + x_recoil[gun_sector][recoil_step],
							  ytmp + dy_laser_flash[gun_sector] + y_recoil[gun_sector][recoil_step], pbk_bb.anim("laser_muzzle")[weapon_step]);
				break;
		}
	}
}

void Ball::renderEye(int xtmp, int ytmp) {
	int delta;

	if (hurt_timer) {
		if (dir <= BBLIM_RIGHT)
			draw(xtmp + 12, ytmp - 14, pbk_own.pic("eye_hurt_right"));
		else
			draw(xtmp - 12, ytmp - 14, pbk_own.pic("eye_hurt_left"));
	} else {
		Flipbook eyes = pbk_bb.anim(is_firing ? "eye_angry" : "eye_normal");

		if (dir <= BBDIR_DOWN_RIGHT)
			delta = 0;
		else if (dir <= BBDIR_RIGHT)
			delta = 1;
		else if (dir <= BBDIR_UP_U_R)
			delta = 2;
		else if (dir <= BBDIR_UP_LEFT)
			delta = 3;
		else if (dir <= BBDIR_LEFT)
			delta = 4;
		else
			delta = 5;

		if (dir <= BBLIM_RIGHT)
			draw(xtmp + 12, ytmp - 14, eyes[delta]);
		else
			draw(xtmp - 12, ytmp - 14, eyes[delta]);
	}
}

void Ball::renderJump() {
	Sprite::render();

	if (dy < 0) {
		renderEye(x, y + dy - 2);
		renderWeapon(x, y + dy - 2);
	} else {
		renderEye(x, y);
		renderWeapon(x, y);
	}
}

void Ball::update() {
	if (phase) {
		blink_step += 1;
		blink_step %= 4;
	}

	if (weapon_id == ID_LF) {
		if (is_firing && !fire_lf) {
			sbk_bb.play(2, SOUND_LOOP);
			fire_lf = true;
		} else if (!is_firing && fire_lf) {
			sbk_bb.stop(2);
			fire_lf = false;
		}
	} else if (fire_lf) {
		sbk_bb.stop(2);
		fire_lf = false;
	}

	if (weapon_id == ID_LASER) {
		if (is_firing && !fire_laser) {
			sbk_bb.play(4, SOUND_LOOP);
			fire_laser = true;
		} else if (!is_firing && fire_laser) {
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
		destroyed = true;
	}

	if (status == STATE_DYING) {
		onDie();
		return;
	}

	if (status == STATE_COME_BACK) {
		onComeBack();
		return;
	}

	if (wait_cow_bomb < 100) wait_cow_bomb += 1;

	if (ctrl->super() && wait_cow_bomb == 100 && nb_cow_bomb >= 1) {
		ShotBBCow* ptr;

		for (int i = 0; i < 11; i++) {
			int xCow = offset + i * 64 + rand() % 20;

			if (!solid_wall(xCow, 0)) {
				ptr = new ShotBBCow();

				ptr->x = xCow;
				ptr->y = rand() % 180 - 200;
				ptr->dy = 5 + rand() % 3;
				ptr->anim_step = rand() % 8;
				ptr->hero = this;

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
	if (x < offset + 20 && !ctrl->rightHeld() && !solid_wall(x + 2, y)) {
		x += 2;

		if (slide_delay < 0) dx_slide = slide_delay = 0;
	}

	int yp = platform_y(x, y);

	if (yp != 0) y = yp;

	// A player pinned against a wall by the scroll dies
	if (x < offset - 30) {
		no_scroll1 = true;
		nb_life -= 1;

		if (nb_life == 0) {
			destroyed = true;
			return;
		}

		status = STATE_COME_BACK;

		x = offset + 320;
		int i = 0;

		while (i < NB_MAX_PLATFORMS && y_platform[i][x] == 0xFFFF) i++;

		y_to_go = i < NB_MAX_PLATFORMS ? y_platform[i][x] : 0xFFFF;

		y = -50;
		col_on = true;
		hp = 5;
		weapon_dir = dir = BBDIR_RIGHT;
		invincible = 400;
		weapon_id = ID_M16;
		weapon_anim_delay = 3;
		nb_weapon_steps = 5;
		fire_interval = 10;
		gun_turn_delay = 1;
		ammo = 0;
		pic = pbk_own.anim("jump_right")[2];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_jump = 0;
		is_firing = false;
	}

	if (invincible > 0) {
		invincible -= 1;
	} else {
		inv_cow = false;
	}

	if (hurt_timer > 0) hurt_timer -= 1;

	if (anim_tick == 0) {
		hop_idx++;
		hop_idx %= 4;
	}

	// Detect a quick double tap on down, which drops through the platform
	if (ctrl->downHeld()) {
		if (time_down > 0 && time_down < 10 && status == STATE_NORMAL)
			time_down = -1;
		else
			time_down = 0;
	} else {
		time_down += 1;
	}

	if (status != STATE_LOCKED_VEHICLE || !locked_dir) manageDirection();

	switch (status) {
		case STATE_NORMAL:
			onNormal();
			break;

		case STATE_WALK:
			onWalk();
			break;

		case STATE_SOMERSAULT:
			onSomersault();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			onJump();
			break;

		case STATE_LOCKED_VEHICLE:
			onVehicle();
			break;
	}

	// Ice keeps some momentum
	if (status == STATE_NORMAL || status == STATE_WALK) {
		int nplatform = platform_y2(x, y);

		if (nplatform >= 6 && x > 10 && x > offset + 30 && x < offset + 610) {
			int ytmp1 = y_platform[nplatform][x + 10] - y;
			int ytmp2 = y_platform[nplatform][x - 10] - y;

			if (ytmp1 < 10 && ytmp1 > -10) slide_delay += ytmp1;
			if (ytmp2 < 10 && ytmp2 > -10) slide_delay -= ytmp2;

			if (slide_delay > 0) {
				if (ctrl->rightHeld())
					slide_delay += 25;
				else if (ctrl->leftHeld())
					slide_delay -= 10;
				else
					slide_delay -= 1;
			} else if (slide_delay < 0) {
				if (ctrl->leftHeld())
					slide_delay -= 25;
				else if (ctrl->rightHeld())
					slide_delay += 10;
				else
					slide_delay += 1;
			} else {
				if (ctrl->rightHeld())
					slide_delay += 25;
				else if (ctrl->leftHeld())
					slide_delay -= 25;
			}

			if (slide_delay > 600)
				slide_delay = 600;
			else if (slide_delay < -600)
				slide_delay = -600;

			if (slide_delay > 0)
				dx_slide = slide_delay / 200 + 1;
			else if (slide_delay < 0)
				dx_slide = slide_delay / 200 - 1;
			else
				dx_slide = 0;
		} else {
			dx_slide = slide_delay = 0;
		}
	} else {
		dx_slide = slide_delay = 0;
	}

	colFromPic();

	if (status != STATE_SOMERSAULT) updateWeapon();

	return;
}

void Ball::onNormal() {
	if (time_down == -1 && !solid_wall(x, y + 5) && !solid_wall(x, y + 10) && !solid_wall(x, y + 15) && !solid_wall(x, y + 20) && !solid_wall(x, y + 25)) {
		y += 10;
		dy = 5;
		status = STATE_FALL;
		grav_tick = 0;
		dx_jump = dx_slide;
		time_down = 0;
		onJump();
		return;
	}

	if (ctrl->jump() && !solid_wall(x, y - 5)) {
		status = STATE_JUMP;
		dy = -5;
		grav_tick = 0;	// Otherwise jump heights vary

		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		jump_released = false;
		onJump();
		return;
	}

	if (ctrl->leftHeld() || ctrl->rightHeld() || dx_slide != 0) {
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		return;
	}

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[animate(anim_walk, 4, 8)];
	else
		pic = pbk_own.anim("walk_left")[animate(anim_walk, 4, 8)];
}

void Ball::onWalk() {
	if (ctrl->jump() && !solid_wall(x, y - 5)) {
		status = STATE_JUMP;
		dy = -5;
		grav_tick = 0;	// Otherwise jump heights vary

		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		jump_released = false;
		onJump();
		return;
	}

	int destx = x + dx_slide;

	if ((dx_slide > 0 && !solid_wall(destx, y) && destx < offset + 620) || (dx_slide < 0 && !solid_wall(destx, y) && destx > offset + 20))
		walk(dx_slide);
	else {
		if (ctrl->rightHeld() && (!solid_wall(x + 2, y) && x + 2 < offset + 620)) {
			walk(2);
		} else if (ctrl->leftHeld() && (!solid_wall(x - 2, y) && x - 2 > offset + 20)) {
			walk(-2);
		} else {
			status = STATE_NORMAL;
		}
	}

	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		onJump();
		return;
	}

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[animate(anim_walk, 4, 5)];
	else
		pic = pbk_own.anim("walk_left")[animate(anim_walk, 4, 5)];
}

void Ball::onJump() {
	int yp;

	// A somersault needs jump released and pressed again mid jump
	if (!ctrl->jump()) jump_released = true;

	if (ctrl->jump() && jump_released && status == STATE_JUMP && dy < 3) {
		dy = -5;
		status = STATE_SOMERSAULT;
		anim_step = 0;
		anim_tick = 0;
		is_firing = false;
		weapon_step = weapon_tick = 0;
		onSomersault();
		return;
	}

	fall2();

	// Releasing jump early makes a shorter jump
	if (dy < 0 && !ctrl->jump()) {
		grav_tick += 1;
		grav_tick %= GRAVITY_DELAY;

		if (grav_tick == 0) {
			dy += 1;
		}
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		slide_delay = 200 * dx_jump;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (!ctrl->inertia() || (!ctrl->downHeld() && !ctrl->upHeld())) {
		if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;
		else if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (!ctrl->inertia())
			dx_jump = 0;
	}

	int xx = x + dx_jump;

	if (!solid_wall(xx, y) && xx > offset + 20 && xx < offset + 620) x = xx;

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("jump_right")[anim_jump[dy + 5]];
	else
		pic = pbk_own.anim("jump_left")[anim_jump[dy + 5]];
}

void Ball::manageDirection() {
	// The pivot is the gun height, the feet are at y. No aim returns to horizontal
	int vx, vy;
	ctrl->aim(x - offset, y - 12, vx, vy);
	double t = std::atan2(-vy, std::abs(vx));
	int s = (int)std::lround(t / (std::numbers::pi / 8));
	bool right = dir <= BBLIM_RIGHT;
	// Straight up or down keeps the facing, so the body does not flip while aiming there
	if (std::abs(s) < 4 && vx != 0) right = vx > 0;
	dir = right ? BBDIR_RIGHT + s : BBDIR_LEFT - s;
	double deg = t * 180 / std::numbers::pi;
	// Mirrored by the aim itself, then kept in the facing's range so near vertical aim stays on its side
	aim_angle_ = vx < 0 ? 180 - deg : deg;
	if (right && aim_angle_ > 180) aim_angle_ -= 360;
	if (!right && aim_angle_ < 0) aim_angle_ += 360;

	if (dir <= BBLIM_RIGHT && weapon_dir > BBLIM_RIGHT)
		weapon_dir = BBDIR_DOWN_D_L - weapon_dir;
	else if (dir > BBLIM_RIGHT && weapon_dir <= BBLIM_RIGHT)
		weapon_dir = 17 - weapon_dir;

	gun_turn_tick += 1;
	gun_turn_tick %= gun_turn_delay;

	if (gun_turn_tick == 0) {
		if (weapon_dir < dir)
			weapon_dir += 1;
		else if (weapon_dir > dir)
			weapon_dir -= 1;
	}
}

void Ball::updateWeapon() {
	if (status == STATE_LOCKED_VEHICLE) return;

	int yback = y;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			y -= hop_offsets[hop_idx];
			break;

		case STATE_JUMP:
		case STATE_FALL:
			if (dy < 0) y += dy - 2;
			break;
	}

	if (is_firing) {
		weapon_tick += 1;
		weapon_tick %= weapon_anim_delay;

		if (weapon_tick == 0) weapon_step += 1;

		if (weapon_step == nb_weapon_steps) is_firing = false;
	}

	if (!is_firing && ctrl->fire() && !hold_fire) {
		is_firing = true;
		weapon_step = 0;
		weapon_tick = 0;

		switch (weapon_id) {
			case ID_M16:
				break;

			case ID_PM:
				sbk_bb.play(0);
				break;

			case ID_LF:
				break;

			case ID_SHOTGUN:
				break;

			case ID_LASER:
				break;
		}

		gun_fire_tick = fire_interval - 1;
	}

	recoil_step += 1;
	recoil_step %= 5;

	gun_fire_tick += 1;
	gun_fire_tick %= fire_interval;

	if (is_firing && gun_fire_tick == 0 && !(gun_turn_delay <= 1 && weapon_dir != dir)) {
		int gun_sector;
		double rot;
		gunSector(gun_sector, rot);
		// Muzzle of the rotated gun, from the sprite's grip
		auto muzzle = [&](ShotBB* t, const int* gdx, const int* gdy, int mx, int my) {
			t->x = x + mx;
			t->y = y + my;
			rotateAround(t->x, t->y, x + gdx[gun_sector], y + gdy[gun_sector], rot);
		};

		ShotBB* t = NULL;
		int i;
		// Drawn y first in separate statements, the order MSVC used inside the old calls, so all builds match
		int jitter_x = 0, jitter_y = 0;

		switch (weapon_id) {
			case ID_M16:

				sbk_bb.play(3);

				next_m16 = (next_m16 + 1) % NB_SHOTS_M16;
				t = &shots_m16[next_m16];
				t->destroyed = false;

				t->hero = this;
				jitter_y = rand() % 11 - 5;
				jitter_x = rand() % 11 - 5;
				muzzle(t, dx_m16, dy_m16, dx_shot_m16[gun_sector] + jitter_x, dy_shot_m16[gun_sector] + jitter_y);

				if (status == STATE_LOCKED_VEHICLE)
					t->dxScroll = scroll_speed;
				else
					t->dxScroll = 0;

				t->setAngle(gunAngle(), 9);
				list_bb_shots.push_back(t);
				break;

			case ID_PM:

				next_pm = (next_pm + 1) % NB_SHOTS_PM;
				t = &shots_pm[next_pm];
				t->destroyed = false;

				t->hero = this;
				jitter_y = rand() % 11 - 5;
				jitter_x = rand() % 11 - 5;
				muzzle(t, dx_pm, dy_pm, dx_shot_pm[gun_sector] + jitter_x, dy_shot_pm[gun_sector] + jitter_y);

				if (status == STATE_LOCKED_VEHICLE)
					t->dxScroll = scroll_speed;
				else
					t->dxScroll = 0;

				t->setAngle(gunAngle(), 12);
				list_bb_shots.push_back(t);
				break;

			case ID_SHOTGUN:

				sbk_bb.play(1);

				for (i = 0; i < 17; i++) {
					next_shotgun = (next_shotgun + 1) % NB_SHOTS_SHOTGUN;
					ShotBBShotgun* pellet = &shots_shotgun[next_shotgun];
					pellet->destroyed = false;

					pellet->hero = this;
					jitter_y = rand() % 11 - 5;
					jitter_x = rand() % 11 - 5;
					muzzle(pellet, dx_shotgun, dy_shotgun, dx_shot_shotgun[gun_sector] + jitter_x, dy_shot_shotgun[gun_sector] + jitter_y);

					if (status == STATE_LOCKED_VEHICLE)
						pellet->dxScroll = scroll_speed;
					else
						pellet->dxScroll = 0;

					pellet->aim(gunAngle());
					list_bb_shots.push_back(pellet);
				}
				break;

			case ID_LASER: {
				next_laser = (next_laser + 1) % NB_SHOTS_LASER;
				ShotBBLaser* beam = &shots_laser[next_laser];
				beam->destroyed = false;

				beam->hero = this;
				muzzle(beam, dx_laser, dy_laser, dx_shot_laser[gun_sector] + x_recoil[gun_sector][recoil_step],
					   dy_shot_laser[gun_sector] + y_recoil[gun_sector][recoil_step]);
				beam->aim(gunAngle());
				beam->anim_step = weapon_step;
				list_bb_shots.push_back(beam);
				break;
			}

			case ID_LF: {
				ShotBBLF* puff;
				next_lf = (next_lf + 1) % NB_SHOTS_LF;
				puff = &shots_lf[next_lf];
				puff->destroyed = false;

				puff->deg = 2;
				puff->hero = this;
				muzzle(puff, dx_lf, dy_lf, dx_shot_lf[gun_sector] + x_recoil[gun_sector][recoil_step],
					   dy_shot_lf[gun_sector] + y_recoil[gun_sector][recoil_step]);
				puff->aim(gunAngle());

				list_bb_shots.push_back(puff);

				next_lf = (next_lf + 1) % NB_SHOTS_LF;
				puff = &shots_lf[next_lf];
				puff->destroyed = false;

				puff->deg = 1;
				puff->hero = this;
				muzzle(puff, dx_lf, dy_lf, dx_shot_lf[gun_sector] + x_recoil[gun_sector][recoil_step],
					   dy_shot_lf[gun_sector] + y_recoil[gun_sector][recoil_step]);
				puff->aim(gunAngle());

				list_bb_shots.push_back(puff);
				break;
			}
		}

		if (!hasUnlimitedAmmo()) {
			ammo -= 1;

			if (ammo <= 0) {
				weapon_id = ID_M16;
				weapon_anim_delay = 3;
				nb_weapon_steps = 5;
				fire_interval = 10;
				gun_turn_delay = 1;
				is_firing = false;
			}
		}
	}

	y = yback;
}

void Ball::takeHit(const Shot* shot) {}

void Ball::takeHit(int damage) {
	if (invincible > 0 || status == STATE_DYING) return;

	hp -= damage;
	perfect = false;
	if (hp <= 0) {
		hp = 0;
		status = STATE_DYING;
		is_firing = false;
		weapon_step = weapon_tick = 0;
		anim_step = 0;
		anim_tick = 0;
		col_on = false;
		no_scroll1 = true;
	} else {
		invincible = 200;
		hurt_timer = 100;
	}
}

void Ball::onDie() {
	if (dy < 0) dy = 0;

	fall();

	anim_tick += 1;
	anim_tick %= 7;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 15) {
		nb_life -= 1;

		if (nb_life == 0) {
			destroyed = true;
			no_scroll1 = false;
			return;
		}

		status = STATE_COME_BACK;
		y_to_go = y;
		y = -50;
		col_on = true;
		hp = 5;
		weapon_dir = dir = BBDIR_RIGHT;
		invincible = 400;

		rearm();

		pic = pbk_own.anim("jump_right")[2];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_jump = 0;
		is_firing = false;

		if (cowBombOn) nb_cow_bomb = 1;
	} else if (anim_step == 5 && anim_tick == 0 && platform_y(x, y) != 0) {
		if (dir <= BBLIM_RIGHT)
			engrave(x, y, pbk_own.pic("grave_right"));
		else
			engrave(x, y, pbk_own.pic("grave_left"));
	} else if (anim_step < 5) {
		if (dir <= BBLIM_RIGHT)
			pic = pbk_own.anim("die_right")[anim_step];
		else
			pic = pbk_own.anim("die_left")[anim_step];
	}
}

void Ball::renderDying() {
	if (anim_step < 5) Sprite::render();
}

void Ball::onComeBack() {
	no_scroll1 = true;

	if (y < y_to_go) {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 8;
		}

		y += 5;

		pic = pbk_own.anim("spin")[weapon_id * 8 + anim_step];
	} else {
		y = y_to_go;
		status = STATE_NORMAL;
		anim_step = 0;
		anim_tick = 0;
		no_scroll1 = false;
	}
}

void Ball::onSomersault() {
	int yp;

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	fall2();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		slide_delay = 200 * dx_jump;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (ctrl->rightHeld() && dx_jump < 2)
		dx_jump = 2;
	else if (ctrl->leftHeld() && dx_jump > -2)
		dx_jump = -2;

	int xx = x + dx_jump;

	if (!solid_wall(xx, y) && xx > offset + 20 && xx < offset + 620) x = xx;

	pic = pbk_own.anim("spin")[weapon_id * 8 + anim_step];
}

bool Ball::okBonus() {
	return (weapon_id == ID_M16 || (weapon_id == ID_PM && ammo <= 30) || (weapon_id == ID_SHOTGUN && ammo <= 4) || (weapon_id == ID_LF && ammo <= 100) ||
			(weapon_id == ID_LASER && ammo <= 100));
}

void Ball::setSuperWeapon() {
	if (id_ball == ID_BLIP || !okFlamethrower) {
		weapon_id = ID_LASER;
		ammo = 750;
		weapon_anim_delay = 3;
		nb_weapon_steps = 4;
		fire_interval = 1;
		gun_turn_delay = 1;

		weapon_step = 0;
		weapon_tick = 0;
		recoil_step = 0;
		is_firing = false;
	} else {
		weapon_id = ID_LF;
		ammo = 1000;
		weapon_anim_delay = 3;
		nb_weapon_steps = 5;
		fire_interval = 1;
		gun_turn_delay = 5;

		weapon_step = 0;
		weapon_tick = 0;
		recoil_step = 0;
		is_firing = false;
	}
}

void Ball::colFromPic() {
	Sprite::colFromPic();

	if (status == STATE_SOMERSAULT) {
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

void Ball::lockVehicle(bool can_fire, bool can_dir) {
	status = STATE_LOCKED_VEHICLE;
	locked_fire = !can_fire;
	locked_dir = !can_dir;

	if (!can_fire) {
		is_firing = false;
		weapon_step = weapon_tick = 0;
	}
}

void Ball::unlockVehicle() {
	anim_step = anim_tick = 0;
	status = STATE_SOMERSAULT;
	dy = -5;
	dx_jump = 0;
	is_firing = false;
	weapon_step = weapon_tick = 0;
}

void Ball::onVehicle() {}

void Ball::renderVehicle() {
	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[0];
	else
		pic = pbk_own.anim("walk_left")[0];

	Sprite::render();
	renderEye(x, y);

	renderWeapon(x, y);
}
