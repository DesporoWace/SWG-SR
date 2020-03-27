#include "LovedayCardMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"

void LovedayCardMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	menuResponse->addRadialMenuItem(20,3,"Combine Cards");
}

int LovedayCardMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(player)) {
		player->sendSystemMessage("The Festival of Love cards can't be used unless they're in your inventory.");
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
	TangibleObject* partFive = NULL;
	TangibleObject* partSix = NULL;
	TangibleObject* partSeven = NULL;

	if (selectedID == 20) {
		for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
			TangibleObject* item = inventory->getContainerObject(i).castTo<TangibleObject*>();

			if (item == NULL) {
				continue;
			}

			if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_01.iff") {
				if (partOne == NULL) {
					partOne = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_02.iff") {
				if (partTwo == NULL) {
					partTwo = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_03.iff") {
				if (partThree == NULL) {
					partThree = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_04.iff") {
				if (partFour == NULL) {
					partFour = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_05.iff") {
				if (partFive == NULL) {
					partFive = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_06.iff") {
				if (partSix == NULL) {
					partSix = item;
				}
			} else if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/holiday/love_day/love_day_card_07.iff") {
				if (partSeven == NULL) {
					partSeven = item;
				}
			}
		}

		if (partOne == NULL || partTwo == NULL || partThree == NULL || partFour == NULL || partFive == NULL || partSix == NULL || partSeven == NULL) {
			player->sendSystemMessage("You do not have all seven Festival of Love cards.");
			return 0;
		}

		Locker pOneLocker(partOne);
		Locker pTwoLocker(partTwo);
		Locker pThreeLocker(partThree);
		Locker pFourLocker(partFour);
		Locker pFiveLocker(partFive);
		Locker psixLocker(partSix);
		Locker pSevenLocker(partSeven);

		String fullTemplate = "object/tangible/holiday/love_day/love_day_card_stack.iff";
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
		partFive->destroyObjectFromWorld(true);
		partFive->destroyObjectFromDatabase();
		partSix->destroyObjectFromWorld(true);
		partSix->destroyObjectFromDatabase();
		partSeven->destroyObjectFromWorld(true);
		partSeven->destroyObjectFromDatabase();

		inventory->transferObject(assembledMessage, -1, true);

		assembledMessage->sendTo(player, true);

		player->sendSystemMessage("You've successfully combined the Festival of Love cards into a deck.");
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
