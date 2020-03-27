/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#include "server/zone/objects/intangible/ShipControlDevice.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/player/PlayerManager.h"

void ShipControlDeviceImplementation::generateObject(CreatureObject* player) {
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == NULL)
		return;

	if (player->hasState(CreatureState::PILOTINGSHIP)) {
		player->clearState(CreatureState::PILOTINGSHIP);
		updateStatus(0);
	} else if (!ghost->isAdmin()) {
		// Player has a ship, delete it and all ships in their datapad
		ManagedReference<SceneObject*> datapad = player->getSlottedObject("datapad");

		if (datapad == NULL)
			return;

		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		for (int i = 0; i < datapad->getContainerObjectsSize(); i++) {
			Reference<SceneObject*> obj =  datapad->getContainerObject(i).castTo<SceneObject*>();

			if (obj != NULL && obj->isShipControlDevice()) {
				player->sendSystemMessage("Removed deactivated Ship Control device from datapad.");
				obj->destroyObjectFromWorld(true);
				obj->destroyObjectFromDatabase(true);
			}
		}
	}

	if (true) {
		player->sendSystemMessage("Atmospheric flight is currently disabled.");
		return;
	}

	// Stop Here

	if (!isClientObject()) { //remove this segment to enable atmospheric flight.
		player->sendSystemMessage("Atmospheric flight is currently disabled.");
		return;
	}

	if (player->isDead() || player->isIncapacitated()) {
		player->sendSystemMessage("You can't call a ship right now.");
		return;
	}

	if (!isClientObject()) {
		if (ghost != NULL && ghost->hasPvpTef()) {
			player->sendSystemMessage("You can't call a ship while TEF.");
			return;
		}
	}

	if (!isASubChildOf(player))
		return;

	if (player->getParent() != NULL) {
		player->sendSystemMessage("You can only unpack vehicles while outside."); // "@pet/pet_menu:cant_call_vehicle" - You can only unpack vehicles while Outside and not in Combat.
		return;
	}

	ZoneServer* zoneServer = getZoneServer();

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	Locker clocker(controlledObject, player);

	controlledObject->initializePosition(player->getPositionX(), player->getPositionZ() + 10, player->getPositionY());

	player->getZone()->transferObject(controlledObject, -1, true);
	//controlledObject->insertToZone(player->getZone());

	//removeObject(controlledObject, true);

	controlledObject->transferObject(player, 5, true);
	player->setState(CreatureState::PILOTINGSHIP);
	//controlledObject->inflictDamage(player, 0, System::random(50), true);

	updateStatus(1);

	if (ghost != NULL)
		ghost->setTeleporting(true);
}

void ShipControlDeviceImplementation::storeObject(CreatureObject* player, bool force) {
	player->clearState(CreatureState::PILOTINGSHIP);

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL)
		return;

	Locker clocker(controlledObject, player);

	if (!controlledObject->isInQuadTree())
		return;

	Zone* zone = player->getZone();

	if (zone == NULL)
		return;

	zone->transferObject(player, -1, false);

	controlledObject->destroyObjectFromWorld(true);

	transferObject(controlledObject, 4, true);

	updateStatus(0);
}

void ShipControlDeviceImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	//ControlDeviceImplementation::fillObjectMenuResponse(menuResponse, player);

	ManagedReference<TangibleObject*> controlledObject = this->controlledObject.get();

	if (controlledObject == NULL)
		return;

	if (!controlledObject->isInQuadTree()) {
		menuResponse->addRadialMenuItem(60, 3, "Launch Ship"); //Launch
	} else
		menuResponse->addRadialMenuItem(61, 3, "Land Ship"); //Land
}

bool ShipControlDeviceImplementation::canBeTradedTo(CreatureObject* player, CreatureObject* receiver, int numberInTrade) {
	PlayerObject* ghost = player->getPlayerObject();
	PlayerObject* ghostReceiver = receiver->getPlayerObject();

	if (ghost == NULL || ghostReceiver == NULL)
		return false;

	if (player->hasState(CreatureState::PILOTINGSHIP)) {
		player->clearState(CreatureState::PILOTINGSHIP);
		updateStatus(0);
	} else if (receiver->hasState(CreatureState::PILOTINGSHIP)) {
		receiver->clearState(CreatureState::PILOTINGSHIP);
		updateStatus(0);
	}

	ManagedReference<SceneObject*> datapad = receiver->getSlottedObject("datapad");

	if (datapad == NULL)
		return false;

	// Check if either player is an admin, if so, allow trade
	if (ghost->isAdmin() || ghostReceiver->isAdmin()) {
		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		int shipsInDatapad = numberInTrade;
		int maxStoredShips = playerManager->getBaseStoredShips();

		for (int i = 0; i < datapad->getContainerObjectsSize(); i++) {
			Reference<SceneObject*> obj =  datapad->getContainerObject(i).castTo<SceneObject*>();

			if (obj != NULL && obj->isShipControlDevice() ){
				shipsInDatapad++;
			}
		}

		if( shipsInDatapad >= maxStoredShips){
			player->sendSystemMessage("That person has too many ships in their datapad");
			receiver->sendSystemMessage("You already have the maximum number of ships that you can own.");
			return false;
		}

		return true;
	} else {
		player->sendSystemMessage("You cannot trade a ship control device that's in your datapad.");
		return false;
	}
}
