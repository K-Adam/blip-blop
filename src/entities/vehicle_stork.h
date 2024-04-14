#pragma once

#include "entities/vehicle.h"

class VehicleStork : public Vehicle {
   public:
	bool hasBeenUsed;

	VehicleStork();

	virtual bool canFire() { return false; };

	virtual bool canChangeDir() { return false; };

	virtual void updateUsed();
	virtual void updateNotUsed();
	virtual void affiche();
};
