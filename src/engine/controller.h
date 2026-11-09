#pragma once

class Controller {
   protected:
	bool inertia_on;

   public:
	Controller() : inertia_on(true){};

	virtual int leftHeld() const = 0;
	virtual int upHeld() const = 0;
	virtual int rightHeld() const = 0;
	virtual int downHeld() const = 0;
	virtual int fire() const = 0;
	virtual int jump() const = 0;
	virtual int super() const = 0;

	// Aim vector of any length from the player's screen position, the move keys give 8 directions
	virtual void aim(int px, int py, int& vx, int& vy) const {
		vx = rightHeld() ? 1 : leftHeld() ? -1 : 0;
		vy = upHeld() ? -1 : downHeld() ? 1 : 0;
	}

	virtual bool inertia() const { return inertia_on; };

	virtual void setInertia(bool i) { inertia_on = i; };
};
