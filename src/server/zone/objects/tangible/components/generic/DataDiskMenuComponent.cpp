/*
 * DataDiskMenuComponent
 *
 *  Created on: 24/6/2019
 *      Author: Tyclo
 */

#include "DataDiskMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/components/generic/DataDiskDataComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
//#include "server/zone/objects/player/sui/callbacks/UnlockDataDiskSuiCallback.h"
//#include "server/zone/objects/player/sui/callbacks/DataDiskGetPageSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/DataDiskGetWaypointSuiCallback.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/keypadbox/SuiKeypadBox.h"

void DataDiskMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject,
		ObjectMenuResponse* menuResponse, CreatureObject* player) const {

	if(!sceneObject->isTangibleObject())
		return;

	TangibleObject* tano = cast<TangibleObject*>(sceneObject);
	if(tano == NULL)
		return;

	menuResponse->addRadialMenuItem(20, 3, "Use");

	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);
}

int DataDiskMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if(!sceneObject->isTangibleObject())
		return 0;

	if (!player->isPlayerCreature())
		return 0;

	if (selectedID == 20) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (!sceneObject->isASubChildOf(inventory)) {
			player->sendSystemMessage("You can only use this item from your inventory.");
			return 0;
		}

		if (!sceneObject->isDataDiskObject()) {
			error("Not a Disk Data Object: " + String::valueOf(sceneObject->getServerObjectCRC()));
			return 0;
		}

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return 0;

		if (ghost->hasSuiBoxWindowType(SuiWindowType::DATADISKMESSAGE))
			return 0;

		ManagedReference<TangibleObject*> tano = cast<TangibleObject*>(sceneObject);

		Reference<DataDiskTemplate*> dataDiskTemplate = cast<DataDiskTemplate*>(sceneObject->getObjectTemplate());
		if (dataDiskTemplate == NULL) {
			error("No DataDiskTemplate for: " + String::valueOf(sceneObject->getServerObjectCRC()));
			return 0;
		}

    /*int templateUnlockCode = dataDiskTemplate->getUnlockCode();
		int templatePages = dataDiskTemplate->getPages();
		int objectUnlockCode = -1;
		DataDiskDataComponent* dataDisk = NULL;

		if (templateUnlockCode != 0) {
			DataObjectComponent* data = tano->getDataObjectComponent()->get();

			if (data != NULL && data->isDataDiskData()) {
				dataDisk = cast<DataDiskDataComponent*>(data);
				objectUnlockCode = dataDisk->getUnlockCode();
				if (objectUnlockCode != 1) {
					if (objectUnlockCode == -1)
						dataDisk->setUnlockCode(templateUnlockCode);

					ManagedReference<SuiKeypadBox*> keypadBox = new SuiKeypadBox(player, SuiWindowType::DATADISKMESSAGE);
					keypadBox->setUsingObject(player);
					keypadBox->setForceCloseDisabled();
					keypadBox->setCallback(new UnlockDataDiskSuiCallback(player->getZoneServer(), dataDisk, tano, templateUnlockCode));

					ghost->addSuiBox(keypadBox);
					player->sendMessage(keypadBox->generateMessage());

					return 0;
				}
			}
		}*/
    int templatePages = 0;
		String templateName = tano->getObjectTemplate()->getTemplateFileName();

		// Template is string name
		String title = "@encoded_disk/disk_data:" + templateName + "_name";
		String text = "@encoded_disk/disk_data:"  + templateName + "_message";

		// Build and send SUI to player
		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::DATADISKMESSAGE);

		Vector<String> waypoint = dataDiskTemplate->getWaypoint();

		if (dataDiskTemplate->isWaypointSet() && templatePages == 0) {
			box->setCallback(new DataDiskGetWaypointSuiCallback(player->getZoneServer(), tano, waypoint));
			box->setOkButton(true, "@space/space_interaction:retire_waypoint_btn");
			box->setCancelButton(true, "@ui:close");
		} else if (templatePages > 0) {
			/*box->setCallback(new DataDiskGetPageSuiCallback(player->getZoneServer(), tano, 0, templatePages-1));
			box->setOkButton(true, "@ui:next");
			title += " (1/" + String::valueOf(templatePages) + ")";*/
		} else {
			box->setOkButton(true, "@ui:close");
		}

		box->setCancelButton(false, "");
		box->setPromptTitle(title);
		box->setPromptText(text);
		box->setUsingObject(player);
		box->setForceCloseDisabled();

		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());
		return 0;
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
