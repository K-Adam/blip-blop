#pragma once

#include <vector>

#include "engine/controller.h"
#include "entities/character.h"
#include "entities/player.h"
#include "entities/shots/shot.h"

#include "entities/shots/shot_bb_bullet.h"
#include "entities/shots/shot_bb_shotgun.h"
#include "entities/shots/shot_bb_laser.h"
#include "entities/shots/shot_bb_lf.h"
#include "core/constants.h"

#define NB_SHOTS_M16 16
#define NB_SHOTS_PM 20
#define NB_SHOTS_SHOTGUN 40
#define NB_SHOTS_LASER 2
#define NB_SHOTS_LF 300

class Ball : public Character, public Player {
   public:
	const PictureBank& pbk_own;
	int id_ball;
	bool perfect;

	Controller* ctrl;
	int hop_idx;

	int weapon_id;
	int weapon_dir;
	// A weapon is carried while it has ammo
	int ammo[NB_WEAPONS];
	bool hasUnlimitedAmmo() const { return weapon_id == ID_M16; }
	bool carries(int id) const { return id == ID_M16 || ammo[id] > 0; }
	void equip(int id);
	void dropWeapons();
	void cycleWeapon(int step);

	int weapon_step;  // Muzzle flash step
	int weapon_tick;
	int nb_weapon_steps;
	int weapon_anim_delay;

	int fire_interval;	// Frames between shots
	int gun_fire_tick;	// Shot timer

	int gun_turn_delay;	 // Gun turning delay
	int gun_turn_tick;

	bool is_firing;	 // Currently firing

	int dx_jump;  // Jump inertia
	int dx_slide;
	int slide_delay;
	int recoil_step;

	int invincible;
	bool inv_cow;
	int hurt_timer;
	int blink_step;

	int y_to_go;

	int nb_life;
	int nb_cow_bomb;
	int wait_cow_bomb;
	bool jump_released;
	int time_down;	// Frames since down was last pressed

	bool locked_fire;
	bool locked_dir;

	bool fire_lf;
	bool fire_laser;

	ShotBBM16 shots_m16[NB_SHOTS_M16];
	int next_m16;

	ShotBBPM shots_pm[NB_SHOTS_PM];
	int next_pm;

	ShotBBShotgun shots_shotgun[NB_SHOTS_SHOTGUN];
	int next_shotgun;

	ShotBBLaser shots_laser[NB_SHOTS_LASER];
	int next_laser;

	ShotBBLF shots_lf[NB_SHOTS_LF];
	int next_lf;

   public:
	int mod_life;

	Ball(const PictureBank& pbk);

	virtual void rearm() = 0;

	void render() override;
	virtual void renderNormal();
	virtual void renderJump();
	virtual void renderDying();
	virtual void renderVehicle();
	virtual void renderWeapon(int xtmp, int ytmp);
	virtual void renderEye(int xtmp, int ytmp);
	void update() override;
	virtual void onNormal();
	virtual void onJump();
	virtual void onWalk();
	virtual void onDie();
	virtual void onComeBack();
	virtual void onSomersault();
	virtual void onVehicle();
	virtual void updateWeapon();
	virtual void takeHit(const Shot* shot);
	virtual void takeHit(int damage);
	void colFromPic() override;

	virtual void lockVehicle(bool can_fire, bool can_dir);
	virtual void unlockVehicle();

	virtual void manageDirection();
	virtual bool okBonus();
	virtual void setSuperWeapon();
	virtual void endLevel() {
		sbk_bb.stop(2);
		sbk_bb.stop(4);
	};

   private:
	// Drawn and fired angle of the gun, in degrees counterclockwise from the right
	double gunAngle() const;
	// Hand drawn sprite nearest the gun angle, and what is left to rotate it by
	void gunSector(int& gun_sector, double& residual) const;
	double aim_angle_ = 0;
	bool switch_held_ = false;
};
