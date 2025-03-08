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

	// 8 way aim, -1/0/1 per axis, from the player's screen position
	virtual void aim(int px, int py, int& ax, int& ay) const {
		ax = droite() ? 1 : gauche() ? -1 : 0;
		ay = haut() ? -1 : bas() ? 1 : 0;
	}

	virtual bool inertia() const { return inertia_on; };

	virtual void setInertia(bool i) { inertia_on = i; };
};
