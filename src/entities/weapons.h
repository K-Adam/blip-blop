#pragma once

#include "entities/shots/shot_bb_bullet.h"
#include "entities/shots/shot_bb_laser.h"
#include "entities/shots/shot_bb_lf.h"
#include "entities/shots/shot_bb_shotgun.h"

class Ball;

struct GunPose {
	int sector;		  // Hand drawn sprite nearest the aim
	double residual;  // Left to rotate that sprite by
	double angle;
};

struct WeaponStats {
	const char* icon;
	int anim_steps;
	int fire_interval;
	int turn_delay;
	int low_ammo;	 // Below it the player may receive weapon bonuses
	int loop_sound;	 // Plays while firing, -1 for none
};

class Weapon {
   public:
	explicit Weapon(const WeaponStats& stats) : stats(stats) {}
	virtual ~Weapon() = default;

	virtual void pullTrigger() {}
	virtual void fire(Ball& b, const GunPose& g) = 0;
	virtual void draw(const Ball& b, int x, int y, const GunPose& g) const = 0;

	const WeaponStats stats;
};

class WeaponM16 : public Weapon {
   public:
	WeaponM16();
	void fire(Ball& b, const GunPose& g) override;
	void draw(const Ball& b, int x, int y, const GunPose& g) const override;

   private:
	ShotBBM16 shots_[16];
	int next_ = 0;
};

class WeaponShotgun : public Weapon {
   public:
	WeaponShotgun();
	void fire(Ball& b, const GunPose& g) override;
	void draw(const Ball& b, int x, int y, const GunPose& g) const override;

   private:
	ShotBBShotgun shots_[40];
	int next_ = 0;
};

class WeaponPM : public Weapon {
   public:
	WeaponPM();
	void pullTrigger() override;
	void fire(Ball& b, const GunPose& g) override;
	void draw(const Ball& b, int x, int y, const GunPose& g) const override;

   private:
	ShotBBPM shots_[20];
	int next_ = 0;
};

class WeaponLF : public Weapon {
   public:
	WeaponLF();
	void fire(Ball& b, const GunPose& g) override;
	void draw(const Ball& b, int x, int y, const GunPose& g) const override;

   private:
	ShotBBLF shots_[300];
	int next_ = 0;
};

class WeaponLaser : public Weapon {
   public:
	WeaponLaser();
	void fire(Ball& b, const GunPose& g) override;
	void draw(const Ball& b, int x, int y, const GunPose& g) const override;

   private:
	ShotBBLaser shots_[2];
	int next_ = 0;
};
