#include "level/events/event_vehicle.h"
#include "core/debug.h"
#include "entities/vehicle.h"
#include "entities/vehicle_stork.h"

#define VEHICLEID_STORK 0

void EventVehicle::doEvent() {
	Vehicle* v = nullptr;

	switch (vehicle_id) {
		case VEHICLEID_STORK:
			v = new VehicleStork();
			break;
	}

	if (v != nullptr) {
		v->x = x;
		v->y = y;
		v->dir = dir;

		level().entities.vehicles.push_back(std::unique_ptr<Vehicle>(v));
	} else {
		debug << "Unknown vehicle ID : " << vehicle_id << "\n";
	}
}
