/*
 * VendorTokenMenucomponent.h
 *
 * Author: TheTinyPebble
 *
 */

#include "VendorTokenMenucomponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/sui/callbacks/SplitTokenSuiCallback.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/objects/player/sui/transferbox/SuiTransferBox.h"

void VendorTokenMenucomponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);
	String fullTemplate = sceneObject->getObjectTemplate()->getFullTemplateString();

	if (fullTemplate.contains("col_dust_durni")) {
		menuResponse->addRadialMenuItem(115,3,"Combine Dust");
		menuResponse->addRadialMenuItem(116,3,"Split Dust");
	} else if (fullTemplate.contains("coin") || fullTemplate.contains("currency")) {
		menuResponse->addRadialMenuItem(115,3,"Combine Tokens");
		menuResponse->addRadialMenuItem(116,3,"Split Tokens");
	}
}

int VendorTokenMenucomponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	SceneObject* inventory = player->getSlottedObject("inventory");
	if (inventory == NULL) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(inventory)) {
		player->sendSystemMessage("You can not combine or split items if they are not in your inventory.");
		return 0;
	}

	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == NULL) {
		return 0;
	}

	String fullTemplate = sceneObject->getObjectTemplate()->getFullTemplateString();
	TangibleObject* tano = cast <TangibleObject* >(sceneObject);
	int itemCount = 0;

	if (selectedID == 115) {
		Vector<TangibleObject*> tangibles;
		for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
			TangibleObject* item = inventory->getContainerObject(i).castTo<TangibleObject*>();

			if (item == NULL) {
				continue;
			}

			if (item->getObjectTemplate()->getFullTemplateString() == fullTemplate) {
				Locker locker(item);

				int useCount = item->getUseCount();
				itemCount += (useCount == 0 ? 1 : useCount);

				tangibles.add(item);
			}
		}

		if (tangibles.size() == 1) {
			player->sendSystemMessage("You can't combine a single item.");
			return 0;
		}

		if (itemCount == 0) {
			player->sendSystemMessage("You do not have enough items in your inventory to combine.");
			return 0;
		}

		for (int i = 0; i < tangibles.size(); i++) {
			SceneObject* obj = tangibles.get(i);

			if (obj != NULL) {
				Locker olocker(obj);
				obj->destroyObjectFromWorld(true);
				obj->destroyObjectFromDatabase();
			}
		}

		ManagedReference<TangibleObject*> newToken = player->getZoneServer()->createObject(fullTemplate.hashCode(), 1).castTo<TangibleObject*>();

		if (newToken == NULL) {
			return 0;
		}

		Locker newTokenLocker(newToken);

		inventory->transferObject(newToken, -1, true);

		newToken->sendTo(player, true);
		newToken->setUseCount(itemCount, true);

		player->sendSystemMessage("You've successfully combined your items.");
	}

	if (selectedID == 116) {
		if (tano->getUseCount() == 1) {
			player->sendSystemMessage("You can't split a single item.");
			return 0;
		}

		ManagedReference<SuiTransferBox*> transfer = new SuiTransferBox(player, SuiWindowType::OBJECT_NAME);
		transfer->setPromptTitle("Split Items");
		transfer->setPromptText("Select how many items go into each stack.");
		transfer->addFrom("Original Stack", "0", "0", "1");
		transfer->addTo("New Stack", String::valueOf(tano->getUseCount()), String::valueOf(tano->getUseCount()), "1");
		transfer->setUsingObject(tano);
		transfer->setForceCloseDistance(1024.f);
		transfer->setCallback(new SplitTokenSuiCallback(player->getZoneServer(), sceneObject));

		ghost->addSuiBox(transfer);
		player->sendMessage(transfer->generateMessage());
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
