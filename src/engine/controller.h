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

	virtual bool inertia() const { return inertia_on; };

	virtual void setInertia(bool i) { inertia_on = i; };
};
