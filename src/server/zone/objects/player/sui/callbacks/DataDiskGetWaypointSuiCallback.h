/*
 * DataDiskGetWaypointSuiCallback.h
 *
 *  Created on: 01/7/2019
 *      Author: Tyclo
 */

#ifndef DATADISKGETWAYPOINTSUICALLBACK_H_
#define DATADISKGETWAYPOINTSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/waypoint/WaypointObject.h"
#include "server/zone/Zone.h"

class DataDiskGetWaypointSuiCallback : public SuiCallback {
	ManagedReference<TangibleObject*> tano;
	Vector<String> waypoint;

public:
	DataDiskGetWaypointSuiCallback(ZoneServer* server, TangibleObject* tano, Vector<String> waypoint)
			: SuiCallback(server) {

		this->tano = tano;
		this->waypoint = waypoint;
	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {

		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed || player == NULL)
			return;

		if (tano == NULL)
			return;

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		String planet = waypoint.get(0);
		int wayX = Integer::valueOf(waypoint.get(1));
		int wayY = Integer::valueOf(waypoint.get(2));

		if (planet == "")
			return;

		String name = tano->getCustomObjectName().toString();

		ManagedReference<WaypointObject*> waypointObject = server->createObject(0xc456e788, 1).castTo<WaypointObject*>();

		Locker locker(waypointObject);

		waypointObject->setCustomObjectName(name, false);
		waypointObject->setColor(WaypointObject::COLOR_GREEN);
		waypointObject->setPosition(wayX, 0, wayY);
		waypointObject->setPlanetCRC(planet.hashCode());
		waypointObject->setActive(true);

		ghost->addWaypoint(waypointObject, true, true);

		locker.release();

	}
};

#endif /* DATADISKGETWAYPOINTSUICALLBACK_H_ */
