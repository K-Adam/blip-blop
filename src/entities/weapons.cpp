#include "entities/weapons.h"

#include <cstdlib>

#include "entities/ball.h"

const int anim_flash[] = {0, 1, 2, 1, 0};

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

namespace {

// Draws around the grip, so a rotated gun stays in the hand
struct Pen {
	int gx, gy;
	double rot, turn;

	void put(int px, int py, const Picture* pic) const {
		if (rot == 0) return ::draw(px, py, pic);
		rotateAround(px, py, gx, gy, rot);
		drawRotated(px, py, pic, rot);
	}

	// Shared effects come drawn pointing down only
	void putTurned(int px, int py, const Picture* pic) const {
		rotateAround(px, py, gx, gy, rot);
		drawRotated(px, py, pic, turn);
	}
};

Pen penAt(int gx, int gy, const GunPose& g) { return {gx, gy, g.residual, g.angle + 90}; }

template <class T, std::size_t N>
T* nextShot(T (&pool)[N], int& next, Ball& b) {
	next = (next + 1) % N;
	T* t = &pool[next];
	t->destroyed = false;
	t->hero = &b;
	return t;
}

// Muzzle of the rotated gun, from the sprite's grip
void placeAtMuzzle(ShotBB* t, const Ball& b, const GunPose& g, const int* gdx, const int* gdy, int mx, int my) {
	t->x = b.x + mx;
	t->y = b.y + my;
	rotateAround(t->x, t->y, b.x + gdx[g.sector], b.y + gdy[g.sector], g.residual);
}

int jitter() { return rand() % 11 - 5; }

}  // namespace

WeaponM16::WeaponM16() : Weapon({.icon = "icon_m16", .anim_steps = 5, .fire_interval = 10, .turn_delay = 1, .low_ammo = 0, .loop_sound = -1}) {}

void WeaponM16::fire(Ball& b, const GunPose& g) {
	sbk_bb.play(3);
	ShotBBM16* t = nextShot(shots_, next_, b);
	// Drawn y first in separate statements, the order MSVC used inside the old calls, so all builds match
	int jitter_y = jitter();
	int jitter_x = jitter();
	placeAtMuzzle(t, b, g, dx_m16, dy_m16, dx_shot_m16[g.sector] + jitter_x, dy_shot_m16[g.sector] + jitter_y);
	t->setAngle(g.angle, 9);
	list_bb_shots.push_back(t);
}

void WeaponM16::draw(const Ball& b, int x, int y, const GunPose& g) const {
	int s = g.sector;
	int r = b.recoil_step;
	Pen pen = penAt(x + dx_m16[s], y + dy_m16[s], g);
	if (!b.is_firing) {
		pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_m16")[s]);
		return;
	}
	pen.put(pen.gx + x_recoil[s][r], pen.gy + y_recoil[s][r], b.pbk_own.anim("gun_m16")[s]);
	const int* fx = b.id_ball == ID_BLIP ? dx_m16_flash : dx_m16o_flash;
	const int* fy = b.id_ball == ID_BLIP ? dy_m16_flash : dy_m16o_flash;
	pen.putTurned(x + fx[s] + x_recoil[s][r], y + fy[s] + y_recoil[s][r], pbk_bb.anim("flash_m16")[anim_flash[b.weapon_step]]);
}

WeaponShotgun::WeaponShotgun() : Weapon({.icon = "icon_shotgun", .anim_steps = 15, .fire_interval = 45, .turn_delay = 1, .low_ammo = 4, .loop_sound = -1}) {}

void WeaponShotgun::fire(Ball& b, const GunPose& g) {
	sbk_bb.play(1);
	for (int i = 0; i < 17; i++) {
		ShotBBShotgun* pellet = nextShot(shots_, next_, b);
		int jitter_y = jitter();
		int jitter_x = jitter();
		placeAtMuzzle(pellet, b, g, dx_shotgun, dy_shotgun, dx_shot_shotgun[g.sector] + jitter_x, dy_shot_shotgun[g.sector] + jitter_y);
		pellet->aim(g.angle);
		list_bb_shots.push_back(pellet);
	}
}

void WeaponShotgun::draw(const Ball& b, int x, int y, const GunPose& g) const {
	int s = g.sector;
	int hand = s % 5;
	int step = b.weapon_step;
	Pen pen = penAt(x + dx_shotgun[s], y + dy_shotgun[s], g);
	if (!b.is_firing) {
		pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_shotgun")[s]);
		pen.put(x + dx_hand_shotgun[s], y + dy_hand_shotgun[s], b.pbk_own.anim("shotgun_hand")[hand]);
		return;
	}
	if (step < 5) {	 // Recoil
		pen.put(pen.gx + x_recoil[s][step] * 2, pen.gy + y_recoil[s][step] * 2, b.pbk_own.anim("gun_shotgun")[s]);
		pen.put(x + dx_hand_shotgun[s] + x_recoil_shotgun[s][step], y + dy_hand_shotgun[s] + y_recoil_shotgun[s][step], b.pbk_own.anim("shotgun_hand")[hand]);
		pen.putTurned(x + dx_shotgun_flash[s] + x_recoil_shotgun[s][step], y + dy_shotgun_flash[s] + y_recoil_shotgun[s][step],
					  pbk_bb.anim("flash_shotgun")[anim_flash[step]]);
	} else {  // Reload
		int ea = (step - 5) / 2;
		pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_shotgun")[s]);
		pen.put(x + dx_hand_shotgun[s] + x_recoil_shotgun[s][ea], y + dy_hand_shotgun[s] + y_recoil_shotgun[s][ea], b.pbk_own.anim("shotgun_hand")[hand]);
	}
}

WeaponPM::WeaponPM() : Weapon({.icon = "icon_pm", .anim_steps = 5, .fire_interval = 5, .turn_delay = 5, .low_ammo = 30, .loop_sound = -1}) {}

void WeaponPM::pullTrigger() { sbk_bb.play(0); }

void WeaponPM::fire(Ball& b, const GunPose& g) {
	ShotBBPM* t = nextShot(shots_, next_, b);
	int jitter_y = jitter();
	int jitter_x = jitter();
	placeAtMuzzle(t, b, g, dx_pm, dy_pm, dx_shot_pm[g.sector] + jitter_x, dy_shot_pm[g.sector] + jitter_y);
	t->setAngle(g.angle, 12);
	list_bb_shots.push_back(t);
}

void WeaponPM::draw(const Ball& b, int x, int y, const GunPose& g) const {
	int s = g.sector;
	int r = b.recoil_step;
	Pen pen = penAt(x + dx_pm[s], y + dy_pm[s], g);
	if (!b.is_firing) {
		pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_pm")[s]);
		return;
	}
	pen.put(pen.gx + x_recoil[s][r], pen.gy + y_recoil[s][r], b.pbk_own.anim("gun_pm")[s]);
	pen.put(x + dx_pm_flash[s] + x_recoil[s][r], y + dy_pm_flash[s] + y_recoil[s][r], b.pbk_own.anim("flash_pm")[s * 3 + anim_flash[b.weapon_step]]);
}

WeaponLF::WeaponLF() : Weapon({.icon = "icon_lf", .anim_steps = 5, .fire_interval = 1, .turn_delay = 5, .low_ammo = 100, .loop_sound = 2}) {}

void WeaponLF::fire(Ball& b, const GunPose& g) {
	int r = b.recoil_step;
	// Two puffs per shot, the first one burns harder
	for (int deg : {2, 1}) {
		ShotBBLF* puff = nextShot(shots_, next_, b);
		puff->deg = deg;
		placeAtMuzzle(puff, b, g, dx_lf, dy_lf, dx_shot_lf[g.sector] + x_recoil[g.sector][r], dy_shot_lf[g.sector] + y_recoil[g.sector][r]);
		puff->aim(g.angle);
		list_bb_shots.push_back(puff);
	}
}

void WeaponLF::draw(const Ball& b, int x, int y, const GunPose& g) const {
	int s = g.sector;
	Pen pen = penAt(x + dx_lf[s], y + dy_lf[s], g);
	pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_lf")[s]);
}

WeaponLaser::WeaponLaser() : Weapon({.icon = "icon_laser", .anim_steps = 4, .fire_interval = 1, .turn_delay = 1, .low_ammo = 100, .loop_sound = 4}) {}

void WeaponLaser::fire(Ball& b, const GunPose& g) {
	int r = b.recoil_step;
	ShotBBLaser* beam = nextShot(shots_, next_, b);
	placeAtMuzzle(beam, b, g, dx_laser, dy_laser, dx_shot_laser[g.sector] + x_recoil[g.sector][r], dy_shot_laser[g.sector] + y_recoil[g.sector][r]);
	beam->aim(g.angle);
	beam->anim_step = b.weapon_step;
	list_bb_shots.push_back(beam);
}

void WeaponLaser::draw(const Ball& b, int x, int y, const GunPose& g) const {
	int s = g.sector;
	int r = b.recoil_step;
	Pen pen = penAt(x + dx_laser[s], y + dy_laser[s], g);
	if (!b.is_firing) {
		pen.put(pen.gx, pen.gy, b.pbk_own.anim("gun_laser")[s]);
		return;
	}
	pen.put(pen.gx + x_recoil[s][r], pen.gy + y_recoil[s][r], b.pbk_own.anim("gun_laser")[s]);
	pen.putTurned(x + dx_laser_part[s] + x_recoil[s][r], y + dy_laser_part[s] + y_recoil[s][r], pbk_bb.anim("laser_glow")[b.weapon_step]);
	if (b.weapon_dir == b.dir)
		pen.putTurned(x + dx_laser_flash[s] + x_recoil[s][r], y + dy_laser_flash[s] + y_recoil[s][r], pbk_bb.anim("laser_muzzle")[b.weapon_step]);
}
