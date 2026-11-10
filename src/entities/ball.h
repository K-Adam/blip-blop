#pragma once

#include <array>
#include <memory>
#include <vector>

#include "engine/controller.h"
#include "entities/character.h"
#include "entities/player.h"
#include "entities/shots/shot.h"

#include "entities/weapons.h"
#include "core/constants.h"

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
	const Weapon& weaponAt(int id) const { return *weapons_[id]; }
	Weapon& weapon() { return *weapons_[weapon_id]; }
	const Weapon& weapon() const { return *weapons_[weapon_id]; }

	int weapon_step;  // Muzzle flash step
	int weapon_tick;
	int weapon_anim_delay;

	int gun_fire_tick;	// Shot timer

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
		if (looping_sound_ >= 0) assets().sounds.bb.stop(looping_sound_);
	};

   private:
	// Drawn and fired angle of the gun, in degrees counterclockwise from the right
	double gunAngle() const;
	GunPose gunPose() const;
	double aim_angle_ = 0;
	bool switch_held_ = false;
	int looping_sound_ = -1;
	// Indexed by weapon id
	std::array<std::unique_ptr<Weapon>, NB_WEAPONS> weapons_{std::make_unique<WeaponM16>(), std::make_unique<WeaponShotgun>(), std::make_unique<WeaponPM>(),
															 std::make_unique<WeaponLF>(), std::make_unique<WeaponLaser>()};
};
