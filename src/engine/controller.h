#pragma once

class Controller {
   protected:
	bool inertia_on;

   public:
	Controller() : inertia_on(true){};

	virtual int gauche() const = 0;
	virtual int haut() const = 0;
	virtual int droite() const = 0;
	virtual int bas() const = 0;
	virtual int fire() const = 0;
	virtual int saut() const = 0;
	virtual int super() const = 0;

	// Aim vector of any length from the player's screen position, the move keys give 8 directions
	virtual void aim(int px, int py, int& vx, int& vy) const {
		vx = droite() ? 1 : gauche() ? -1 : 0;
		vy = haut() ? -1 : bas() ? 1 : 0;
	}

	virtual bool inertia() const { return inertia_on; };

	virtual void setInertia(bool i) { inertia_on = i; };
};
