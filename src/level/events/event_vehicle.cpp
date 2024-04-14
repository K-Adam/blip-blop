#include "level/events/event_vehicle.h"
#include "core/debug.h"
#include "entities/vehicle.h"
#include "entities/vehicle_stork.h"

#define IDVEHICULE_CIGO 0

void EventVehicle::doEvent() {
	Vehicle* v = NULL;

	switch (id_vehicule) {
		case IDVEHICULE_CIGO:
			v = new VehicleStork();
			break;
	}

	if (v != NULL) {
		v->x = x;
		v->y = y;
		v->dir = dir;

		list_vehicules.push_back(std::unique_ptr<Vehicle>(v));
	} else {
		debug << "Unknown vehicle ID : " << id_vehicule << "\n";
	}
}
