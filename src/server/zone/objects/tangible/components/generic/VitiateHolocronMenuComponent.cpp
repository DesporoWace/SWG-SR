
#include "VitiateHolocronMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"


void VitiateHolocronMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	menuResponse->addRadialMenuItem(20,3,"Assemble Holocron");

}

int VitiateHolocronMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(player)) {
		player->sendSystemMessage("The holocron splinters can't be assembled unless they're in your inventory");
		return 0;
	}

	TangibleObject* fragment = cast<TangibleObject*>(sceneObject);
	if (fragment == NULL) {
		return 0;
	}
	
	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == NULL) {
		return 0;
	}
	
	SceneObject* inventory = player->getSlottedObject("inventory");
	if (inventory == NULL) {
			return 0;
		}
	
	TangibleObject* partOne = NULL;
	TangibleObject* partTwo = NULL;
	TangibleObject* partThree = NULL;
	TangibleObject* partFour = NULL;

	String usingObjectName = fragment->getCustomObjectName().toString();
	
	/*if (usingObjectName.contains("[Part 1]")) {
		partOne = fragment;
	} else if (usingObjectName.contains("[Part 2]")) {
		partTwo = fragment;
	} else if (usingObjectName.contains("[Part 3]")) {
		partThree = fragment;
	} else if (usingObjectName.contains("[Part 4]")) {
		partFour = fragment;
	}*/
	
	if (selectedID == 20) {
		for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
			TangibleObject* item = inventory->getContainerObject(i).castTo<TangibleObject*>();

			if (item == NULL || item->getObjectTemplate()->getFullTemplateString() != "object/tangible/loot/npc/loot/vitiate_splinters.iff") {
				continue;
			}

			String itemName = item->getCustomObjectName().toString();
			
			if (itemName.contains("[Part 1]")) {
				if (partOne == NULL) {
					partOne = item;
				}
			} else if (itemName.contains("[Part 2]")) {
				if (partTwo == NULL) {
					partTwo = item;
				}
			} else if (itemName.contains("[Part 3]")) {
				if (partThree == NULL) {
					partThree = item;
				}
			} else if (itemName.contains("[Part 4]")) {
				if (partFour == NULL) {
					partFour = item;
				}
			}
		}
		
		if (partOne == NULL || partTwo == NULL || partThree == NULL || partFour == NULL) {
			player->sendSystemMessage("You do not have enough holocron splinters to assemble the holocron.");
			return 0;
		}

		Locker pOneLocker(partOne);
		Locker pTwoLocker(partTwo);
		Locker pThreeLocker(partThree);
		Locker pFourLocker(partFour);
				
		String fullTemplate = "object/tangible/loot/npc/loot/vitiate_holocron.iff";
		ManagedReference<SceneObject*> assembledMessage = player->getZoneServer()->createObject(fullTemplate.hashCode(), 1).castTo<SceneObject*>();
		
		if (assembledMessage == NULL) {
			return 0;
		}

		Locker assembledMessageLocker(assembledMessage);

		partOne->destroyObjectFromWorld(true);
		partOne->destroyObjectFromDatabase();
		partTwo->destroyObjectFromWorld(true);
		partTwo->destroyObjectFromDatabase();
		partThree->destroyObjectFromWorld(true);
		partThree->destroyObjectFromDatabase();
		partFour->destroyObjectFromWorld(true);
		partFour->destroyObjectFromDatabase();
			
		inventory->transferObject(assembledMessage, -1, true);

		assembledMessage->sendTo(player, true);

		player->sendSystemMessage("You've successfully combined the holocron splinters.");
	}
	
	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
