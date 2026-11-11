#pragma once

#include "entities/player.h"
#include "entities/shots/shot.h"

#include "core/constants.h"

class ShotBB : public Shot {
   public:
	Player* hero;
	int dxScroll;

	ShotBB() : dxScroll(0){};
	int damage() const override = 0;
	int enflame() const override = 0;

	void onKill() override { hero->addKilled(); };

	inline void onHit(int pts) override {
		destroyed = true;
		col_on = false;
		hero->addPoints(pts);
	};

	// Exact direction in degrees counterclockwise from the right
	void setAngle(double deg, double speed);

   protected:
	void moveAlongAngle();
	// For sprites drawn pointing down, turned to the shot angle
	void drawAlongAngle() const { drawRotated(x, y, pic, angle_ + 90); }
	void colAlongAngle();
	void push(double ddx, double ddy) {
		fx_ += ddx;
		fy_ += ddy;
	}
	void accelerate(double dvx, double dvy) {
		vx_ += dvx;
		vy_ += dvy;
	}
	double vy() const { return vy_; }

   private:
	double fx_ = 0, fy_ = 0, vx_ = 0, vy_ = 0, angle_ = 0;
};
