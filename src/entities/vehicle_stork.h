#pragma once

#include "entities/vehicle.h"

class VehicleStork : public Vehicle {
   public:
	bool hasBeenUsed;

	VehicleStork();

	bool canFire() override { return false; };

	bool canChangeDir() override { return false; };

	void updateUsed() override;
	void updateNotUsed() override;
	void affiche() override;
};
