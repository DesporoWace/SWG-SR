/*
 * HarvesterResourceExtractor
 *
 *  Created on: 24/03/2019
 *      Author: Tyclo
 */

#include "HarvesterResourceExtractorMenuComponent.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/callbacks/HarvesterResourceExtractorHopperSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/HarvesterResourceExtractorAddFuelSuiCallback.h"
#include "server/zone/objects/installation/harvester/HarvesterObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"

void HarvesterResourceExtractorMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	if (!sceneObject->isTangibleObject())
		return;

	TangibleObject* tano = cast<TangibleObject*>(sceneObject);
	if (tano == NULL)
		return;

	menuResponse->addRadialMenuItem(21, 3, "Add Fuel Cell");

	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);
}

int HarvesterResourceExtractorMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!sceneObject->isTangibleObject())
		return 0;

	if (!player->isPlayerCreature())
		return 0;

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == NULL)
		return 0;

	if (ghost->hasSuiBoxWindowType(SuiWindowType::HARVESTER_COMM))
		return 0;

	if (selectedID == 20 || selectedID == 22) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (!sceneObject->isASubChildOf(inventory)) {
			player->sendSystemMessage("In order to operate the Gowix Harvester Extractor, the item must be in your inventory,");
			return 0;
		}

		TangibleObject* tano = cast<TangibleObject*>(sceneObject);

		Locker locker(tano);
		int useCount = tano->getUseCount();

		// No charges
		if (selectedID == 20 && useCount <= 0) {
			player->sendSystemMessage("The Gowix Harvestor Extractor's charges have been depleted. Use a TSE Fuel Cell to recharge this device.");
			return 0;
		}

		int count = 0;

		ManagedReference<SuiListBox*> suiBox = new SuiListBox(player, SuiWindowType::HARVESTER_COMM, SuiListBox::HANDLETWOBUTTON);
		suiBox->setPromptTitle(player->getFirstName() + "'s Harvesters");
		suiBox->setOkButton(true, "@ui:res_harvest_manage"); // Manage
		suiBox->setCancelButton(true, "@cancel");
		suiBox->setUsingObject(sceneObject);

		for (int i = 0; i < ghost->getTotalOwnedStructureCount(); i++) {
			ManagedReference<StructureObject*> structure = player->getZoneServer()->getObject(ghost->getOwnedStructure(i)).castTo<StructureObject*>();

			if (structure == NULL)
				continue;

			if (structure->isHarvesterObject() || structure->isGeneratorObject()) {
				String menuString = "";
				StringIdManager* sidman = StringIdManager::instance();
				String buildingType = sidman->getStringId("@installation_n:" + structure->getObjectNameStringIdName()).toString();
				String buildingName = structure->getCustomObjectName().toString();

				if (buildingName == "" && buildingType != "")
					buildingName = buildingType;

				String operational = "\\#ff4444(Offline)";

				count++;
				ManagedReference<InstallationObject*> installationObj = structure.castTo<InstallationObject*>();
				if (installationObj->isOperating())
					operational = "\\#47ef77(Online)";

				menuString = " \\#ffe254" + buildingName + " \\#ffff3f- \\#ffffff" + buildingType + " " + operational;
				suiBox->addMenuItem(menuString, structure->getObjectID());
			}
		}

		if (count == 0) {
			player->sendSystemMessage("You do not own any harvesters that are compatible with this device.");
			return 0;
		}

		// Decrease uses if player owns harvesters
		if (selectedID == 20) {
			if (useCount == 1)
				tano->setUseCount(0, true); // Do not destroy if use count is 1
			else
				tano->decreaseUseCount();
		}

		String promptText = "\\#ffffffSelect a harvester and \\#fff1bc\"Manage\" \\#ffffffto view and retrieve the harvevester's resources.";
		promptText += "\n\n\\#cee5e5Harvesters: \\#ffffff" + String::valueOf(count);
		suiBox->setPromptText(promptText);
		suiBox->setCallback(new HarvesterResourceExtractorHopperSuiCallback(player->getZoneServer(), sceneObject));
		suiBox->setForceCloseDisabled();
		ghost->addSuiBox(suiBox);
		player->sendMessage(suiBox->generateMessage());

		return 0;
	} else if (selectedID == 21) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (!sceneObject->isASubChildOf(inventory)) {
			player->sendSystemMessage("In order to operate the Gowix Harvester Extractor, the item must be in your inventory");
			return 0;
		}

		ManagedReference<SuiListBox*> suiBox = new SuiListBox(player, SuiWindowType::HARVESTER_COMM);

		StringIdManager* sidman = StringIdManager::instance();
		int fuelUseCount = 0;
		String fuelCell = "";
		int totalFuelCells = 0;

		for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
			TangibleObject* item = inventory->getContainerObject(i).castTo<TangibleObject*>();

			if (item == NULL) {
				continue;
			}

			if (item->getObjectTemplate()->getFullTemplateString() == "object/tangible/item/tse_fuel_cell.iff") {
				Locker locker(item);

				// Get Name (Might do multiple levels of charges later)
				fuelCell = item->getCustomObjectName().toString();
				if (fuelCell == "")
					fuelCell = sidman->getStringId("@" + item->getObjectNameStringIdFile() + ":" + item->getObjectNameStringIdName()).toString();

				fuelUseCount = item->getUseCount();
				fuelCell = "\\#ffe254" + fuelCell + "\\#ffff3f - \\#ffffff(" + fuelUseCount + " Charges)";
				totalFuelCells++;
				suiBox->addMenuItem(fuelCell, item->getObjectID());
			}
		}

		if (totalFuelCells != 0) {
			String promptText = "\\#ffffffSelect a \\#fff1bcFuel Cell\\#ffffff.\n\nYou may add up to \\#ffd27f100 \\#ffffffcharges to a single Gowix Harvestor Extractor.";

			suiBox->setPromptTitle("Add Fuel Cell");
			suiBox->setOkButton(true, "@ui:add");
			suiBox->setCancelButton(true, "@cancel");
			suiBox->setUsingObject(sceneObject);
			suiBox->setPromptText(promptText);
			suiBox->setCallback(new HarvesterResourceExtractorAddFuelSuiCallback(player->getZoneServer()));
			suiBox->setForceCloseDisabled();
			ghost->addSuiBox(suiBox);
			player->sendMessage(suiBox->generateMessage());
		} else {
			player->sendSystemMessage("You do not have any fuel cells compatible with this device in your inventory.");
		}
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
