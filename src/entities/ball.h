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

	int id_arme;
	int dir_arme;
	int ammo;

	int etape_arme;	 // Pour la détonation
	int ss_etape_arme;
	int nb_etape_arme;
	int latence_arme;

	int cadence_arme;  // Latence tir
	int pruno;		   // Etape tir

	int poid_arme;	// Latence tourne arme
	int etape_bouge_arme;

	bool tire;	// Est en train de tirer

	int dx_saut;  // Pour l'inertie
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
	bool jump_released;	 // Le bouton saut a été relaché
	int time_down;		 // Temps ecoulé depuis la dernière fois
						 // que bas a été pressé

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

	virtual void affiche();
	virtual void afficheNormal();
	virtual void afficheSaute();
	virtual void afficheMeure();
	virtual void afficheVehicule();
	virtual void afficheArme(int xtmp, int ytmp);
	virtual void afficheOeil(int xtmp, int ytmp);
	virtual void update();
	virtual void onNormal();
	virtual void onSaute();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onComeBack();
	virtual void onSaleto();
	virtual void onVehicule();
	virtual void updateArme();
	virtual void estTouche(const Shot* tir);
	virtual void estTouche(int degats);
	virtual void colFromPic();

	virtual void lockVehicule(bool can_fire, bool can_dir);
	virtual void unlockVehicule();

	virtual void manageDirection();
	virtual bool okBonus();
	virtual void setSuperWeapon();
	virtual void endLevel() {
		sbk_bb.stop(2);
		sbk_bb.stop(4);
	};
};
