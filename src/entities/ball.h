#pragma once

#include <vector>

#include "engine/controller.h"
#include "entities/character.h"
#include "entities/player.h"
#include "entities/shots/shot.h"

#include "entities/shots/shot_bb_m16.h"
#include "entities/shots/shot_bb_pm.h"
#include "entities/shots/shot_bb_shotgun.h"
#include "entities/shots/shot_bb_laser.h"
#include "entities/shots/shot_bb_lf.h"
#include "core/constants.h"

#define NB_TIRS_M16 16
#define NB_TIRS_PM 20
#define NB_TIRS_FUSIL 40
#define NB_TIRS_LASER 2
#define NB_TIRS_LF 300

class Ball : public Character, public Player {
   public:
	const PictureBank& pbk_own;
	int id_ball;
	bool perfect;

	Controller* ctrl;
	int sauti;

	int weapon_id;
	int dir_arme;
	int ammo;

	int etape_arme;	 // Muzzle flash step
	int ss_etape_arme;
	int nb_weapon_steps;
	int weapon_anim_delay;

	int fire_interval;	// Frames between shots
	int pruno;			// Shot timer

	int gun_weight;	 // Gun turning delay
	int etape_bouge_arme;

	bool is_firing;	 // Currently firing

	int dx_saut;  // Jump inertia
	int dx_glisse;
	int latence_glisse;
	int etape_recul;

	int invincible;
	bool inv_cow;
	int a_mal;
	int etape_cli;

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

	ShotBBM16 tirs_m16[NB_TIRS_M16];
	int next_m16;

	ShotBBPM tirs_pm[NB_TIRS_PM];
	int next_pm;

	ShotBBShotgun tirs_fusil[NB_TIRS_FUSIL];
	int next_fusil;

	ShotBBLaser tirs_laser[NB_TIRS_LASER];
	int next_laser;

	ShotBBLF tirs_lf[NB_TIRS_LF];
	int next_lf;

   public:
	int mod_life;

	Ball(const PictureBank& pbk);

	virtual void rearme() = 0;

	void render() override;
	virtual void afficheNormal();
	virtual void afficheSaute();
	virtual void afficheMeure();
	virtual void afficheVehicule();
	virtual void afficheArme(int xtmp, int ytmp);
	virtual void afficheOeil(int xtmp, int ytmp);
	void update() override;
	virtual void onNormal();
	virtual void onSaute();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onComeBack();
	virtual void onSaleto();
	virtual void onVehicule();
	virtual void updateArme();
	virtual void takeHit(const Shot* shot);
	virtual void takeHit(int damage);
	void colFromPic() override;

	virtual void lockVehicule(bool can_fire, bool can_dir);
	virtual void unlockVehicule();

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
	void gunSector(int& d_arme, double& residual) const;
	double aim_angle_ = 0;
};
