/*
 * HarvesterResourceExtractorAddFuelSuiCallback.h
 *
 *  Created on: 20/7/2019
 *      Author: Tyclo
 */

#ifndef HARVESTERRESOURCEEXTRACTORADDFUELSUICALLBACK_H_
#define HARVESTERRESOURCEEXTRACTORADDFUELSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"

class HarvesterResourceExtractorAddFuelSuiCallback : public SuiCallback {
public:
	HarvesterResourceExtractorAddFuelSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
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

		if (ghost->hasSuiBoxWindowType(SuiWindowType::HARVESTER_COMM))
			return;

		SuiListBox* listBox = cast<SuiListBox*>(suiBox);
		uint64 fuellCellID = listBox->getMenuObjectID(index);

		ManagedReference<SceneObject*> sceneObject = listBox->getUsingObject().get();
		ManagedReference<TangibleObject*> HRE = sceneObject.castTo<TangibleObject*>();

		ManagedReference<TangibleObject*> fuelCell = server->getObject(fuellCellID).castTo<TangibleObject*>();

		if (fuelCell == NULL)
			return;

		if (!fuelCell->isTangibleObject())
			return;

		if (HRE == NULL)
			return;

		if (!HRE->isTangibleObject())
			return;

		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (!fuelCell->isASubChildOf(inventory) || !HRE->isASubChildOf(inventory)) {
			player->sendSystemMessage("In order to add fuel to the Gowix Harvester Extractor, both the fuel cell and the device must be in your inventory.");
			return;
		}

		Locker hreLocker(HRE);
		int hreUseCount = 0;
		hreUseCount = HRE->getUseCount();

		Locker fuelLocker(fuelCell);
		int fuelUseCount = 0;
		fuelUseCount = fuelCell->getUseCount();

		if (hreUseCount + fuelUseCount > 100) {
			player->sendSystemMessage("Adding this fuel cell would exceed the designed power limits of this device. You may only add up to \\#ffd27f100 \\#ffffffcharges to the Gowix Harvester Extractor.");
			return;
		}

		HRE->setUseCount(hreUseCount + fuelUseCount, true);
		fuelCell->destroyObjectFromWorld(true);
		if (fuelCell->isPersistent())
			fuelCell->destroyObjectFromDatabase(true);

		player->sendSystemMessage("You've successfully added \\#ffd27f" + String::valueOf(fuelUseCount) + " \\#ffffffcharges to the Gowix Harvester Extractor. The device now has \\#ffe254" + String::valueOf(hreUseCount + fuelUseCount) + " \\#ffffffcharges.");

	}
};

#endif /* HARVESTERRESOURCEEXTRACTORADDFUELSUICALLBACK_H_ */
