/*
 * SarlaccTrashCanMenuComponent
 *
 *  Created on: 14/03/2019
 *      Author: Tyclo
 */

#include "server/zone/objects/creature/CreatureObject.h"
#include "SarlaccTrashCanMenuComponent.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/sui/callbacks/SarlaccTrashCanSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/ItemAddChargesSuiCallback.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/managers/stringid/StringIdManager.h"

void SarlaccTrashCanMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {

	if (sceneObject->checkContainerPermission(player, ContainerPermissions::MOVECONTAINER && sceneObject->getParent().get() != NULL &&
		sceneObject->getParent().get()->checkContainerPermission(player, ContainerPermissions::MOVEOUT))) {
		menuResponse->addRadialMenuItem(20, 3, "Consume Items");
		menuResponse->addRadialMenuItem(21, 3, "Feed Sarlacc Chow");
	}

	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);
}

int SarlaccTrashCanMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!sceneObject->isTangibleObject())
		return 0;

	if (!player->isPlayerCreature())
		return 0;

	if (selectedID == 20) {
		int itemCount = 0;
		itemCount = sceneObject->getContainerObjectsSize();
		if (itemCount <= 0) {
			player->sendSystemMessage("Please add an item to the Sarlacc Trash Can to destroy it.");
			return 0;
		}

		if (itemCount > 15) {
			player->sendSystemMessage("Sarlacc Trash Can cannot comsume more than 15 items at a time.");
			return 0;
		}

		TangibleObject* tano = cast<TangibleObject*>(sceneObject);

		Locker locker(tano);
		int useCount = tano->getUseCount();

		// No charges
		if (useCount <= 0) {
			player->sendSystemMessage("The Sarlacc Trash Can lacks the digestive enzymes to cosume these items. Feed it some \\#ffd27fSarlacc Chow \\#ffffffbefore trying again.");
			return 0;
		}

		String itemsText = "";
		String noTradeText = "";
		String itemName = "";
		int noTrade = 0;
		StringIdManager* sidman = StringIdManager::instance();

		for (int i = 0; i < itemCount; ++i) { // Get list of items in trash can
			ManagedReference<SceneObject*> object = sceneObject->getContainerObject(i);
			if (object != NULL) {
				itemName = object->getCustomObjectName().toString();
				if (itemName == "")
					itemName = sidman->getStringId("@" + object->getObjectNameStringIdFile() + ":" + object->getObjectNameStringIdName()).toString();

				if (!object->isNoTrade()) {
					itemsText += "\\#ffffff" + String::valueOf(i + 1) + ") \\#ffd27f" + itemName + "\n";
				} else {
					noTrade++;
					noTradeText += "\\#ffffff" + String::valueOf(noTrade) + ") \\#ff982b" + itemName + "\n";
				}

			} else {
				player->sendSystemMessage("The Sarlacc Trash Can burps up an item that it tried to consume. Nothing has been destroyed. Please report this message.");
				return 0;
			}
		}

		String text = "";
		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::NONE);

		if (noTrade != 0) {
			box->setPromptTitle("Sarlacc Trash Can: Unconsumable Items");
			text = "\\#ff0000[ATTENTION] \\#eeeeeeThe following items cannot be consumed (destroyed) by the Sarlacc Trash Can.\n\n";
			text += noTradeText;
			text += "\n\\#fff1bcRemove these items to use the Sarlacc Trash Can.";
			box->setOkButton(true, "@ui:cancel");
		} else {
			box->setCallback(new SarlaccTrashCanSuiCallback(player->getZoneServer(), sceneObject));
			box->setPromptTitle("Sarlacc Trash Can: Destroy All Items");
			box->setOkButton(true, "@player_structure:delete_all_items");
			box->setCancelButton(true, "@ui:close");
			text = "\\#f6d53bDestroy All Items?\n\n\\#eeeeeeWould you like to feed the Sarlacc Trash Can and \\#ffa500permanently destroy \\#eeeeeethe following \\#ffe254" + String::valueOf(itemCount) + "\\#eeeeee item(s)?\n\n";
			text += itemsText;
			text += "\n\\#ffffffThere is no undoing this action.";
		}

		box->setPromptText(text);
		box->setUsingObject(player);
		box->setForceCloseDisabled();

		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());

		return 0;
	} else if (selectedID == 21) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		ManagedReference<SuiListBox*> suiBox = new SuiListBox(player, SuiWindowType::NONE);

		StringIdManager* sidman = StringIdManager::instance();
		int fuelUseCount = 0;
		String fuelName = "";
		String fuelOption = "";
		int totalFuel = 0;

		for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
			TangibleObject* item = inventory->getContainerObject(i).castTo<TangibleObject*>();

			if (item == NULL) {
				continue;
			}

			if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/item/sarlacc_chow.iff") {
				Locker locker(item);

				// Get Name (Might do multiple levels of charges later)
				fuelName = "";
				fuelOption = "";
				fuelName = item->getCustomObjectName().toString();
				if (fuelName == "")
					fuelName = sidman->getStringId("@" + item->getObjectNameStringIdFile() + ":" + item->getObjectNameStringIdName()).toString();

				fuelUseCount = item->getUseCount();
				fuelOption = "\\#ffe254" + fuelName + "\\#ffff3f - \\#ffffff(" + fuelUseCount + " Charges)";
				totalFuel++;
				suiBox->addMenuItem(fuelOption, item->getObjectID());
			}
		}

		if (totalFuel != 0) {
			String promptText = "\\#ffffffSelect a \\#fff1bc" + fuelName + "\\#ffffff.\n\nYou may add up to \\#ffd27f60 \\#ffffffcharges to your Sarlacc Trash Can.";

			suiBox->setPromptTitle("Feed Sarlacc Chow");
			suiBox->setOkButton(true, "@ui:add");
			suiBox->setCancelButton(true, "@cancel");
			suiBox->setUsingObject(sceneObject);
			suiBox->setPromptText(promptText);
			suiBox->setCallback(new ItemAddChargesSuiCallback(player->getZoneServer(), false, 60));
			suiBox->setForceCloseDisabled();
			player->getPlayerObject()->addSuiBox(suiBox);
			player->sendMessage(suiBox->generateMessage());
		} else {
			player->sendSystemMessage("You do not have any Sarlacc Chow in your inventory.");
		}
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
