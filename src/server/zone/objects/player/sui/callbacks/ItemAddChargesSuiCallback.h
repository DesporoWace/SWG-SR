/*
 * ItemAddChargesSuiCallback.h
 *
 *  Created on: 06/8/2019
 *      Author: Tyclo
 */

#ifndef ITEMADDCHARGESSUICALLBACK_H_
#define ITEMADDCHARGESSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/Zone.h"

class ItemAddChargesSuiCallback : public SuiCallback {
	bool inventoryCheck;
	int maxCharges;

public:
	ItemAddChargesSuiCallback(ZoneServer* server, bool inventoryCheck, int maxCharges)
		: SuiCallback(server) {
			this->inventoryCheck = inventoryCheck;
			this->maxCharges = maxCharges;
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isListBox() || cancelPressed)
			return;

		if (args->size() < 1)
			return;

		int index = Integer::valueOf(args->get(0).toString());

		if (index == -1)
			return;

		if (!player->isPlayerCreature())
			return;

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		SuiListBox* listBox = cast<SuiListBox*>(suiBox);
		uint64 fuelID = listBox->getMenuObjectID(index);

		ManagedReference<SceneObject*> sceneObject = listBox->getUsingObject().get();
		ManagedReference<TangibleObject*> rechargeableObject = sceneObject.castTo<TangibleObject*>();

		ManagedReference<TangibleObject*> fuelObject = server->getObject(fuelID).castTo<TangibleObject*>();

		if (fuelObject == NULL)
			return;

		if (!fuelObject->isTangibleObject())
			return;

		if (rechargeableObject == NULL)
			return;

		if (!rechargeableObject->isTangibleObject())
			return;

		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		StringIdManager* sidman = StringIdManager::instance();
		String rechargeableName = rechargeableObject->getCustomObjectName().toString();
		if (rechargeableName == "")
			rechargeableName = sidman->getStringId("@" + rechargeableObject->getObjectNameStringIdFile() + ":" + rechargeableObject->getObjectNameStringIdName()).toString();

		String fuelName = rechargeableObject->getCustomObjectName().toString();
		if (fuelName == "")
			fuelName = sidman->getStringId("@" + fuelObject->getObjectNameStringIdFile() + ":" + fuelObject->getObjectNameStringIdName()).toString();


		if (inventoryCheck) {
			if (!fuelObject->isASubChildOf(inventory) || !rechargeableObject->isASubChildOf(inventory)) {
				player->sendSystemMessage("In order to recharge this item, both the \\#fff1bc" + rechargeableName + "\\#ffffff and \\#ffd27f" + fuelName + "\\#ffffff must be in your inventory.");
				return;
			}
		} else {
			if (!fuelObject->isASubChildOf(inventory)) {
				player->sendSystemMessage("In order to add charges to the \\#fff1bc" + rechargeableName + ", the item must be in your inventory.");
				return;
			}
		}

		Locker rechargeableLocker(rechargeableObject);
		int useCount = 0;
		useCount = rechargeableObject->getUseCount();

		Locker fuelLocker(fuelObject);
		int fuelUseCount = 0;
		fuelUseCount = fuelObject->getUseCount();

		if (useCount + fuelUseCount > maxCharges) {
			player->sendSystemMessage("You may only add up to \\#ffd27f" + String::valueOf(maxCharges) + " \\#ffffffcharges to the \\#fff1bc" + rechargeableName + "\\#ffffff.");
			return;
		}

		rechargeableObject->setUseCount(useCount + fuelUseCount, true);
		fuelObject->destroyObjectFromWorld(true);
		if (fuelObject->isPersistent())
			fuelObject->destroyObjectFromDatabase(true);

		player->sendSystemMessage("You've successfully added \\#ffd27f" + String::valueOf(fuelUseCount) + " \\#ffffffcharges to your \\#fff1bc" + rechargeableName + "\\#ffffff. Charges have been increased to \\#ffe254" + String::valueOf(useCount + fuelUseCount) + " \\#ffffff.");

	}
};

#endif /* ITEMADDCHARGESSUICALLBACK_H_ */
