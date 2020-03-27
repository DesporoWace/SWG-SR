/*
 * SarlaccTrashCanSuiCallback.h
 *
 *  Created on: 13/7/2019
 *      Author: Tyclo
 */

#ifndef SARLACCTRASHCANSUICALLBACK_H_
#define SARLACCTRASHCANSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"

class SarlaccTrashCanSuiCallback : public SuiCallback {
	ManagedReference<SceneObject*> sceneObject;

public:
	SarlaccTrashCanSuiCallback(ZoneServer* server, SceneObject* sceneObject)
			: SuiCallback(server) {

		this->sceneObject = sceneObject;
	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {

		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed || player == NULL)
			return;

		if (sceneObject == NULL)
			return;

		int itemCount = sceneObject->getContainerObjectsSize();

		if (itemCount == 0)
			return;

		int deletedItems = 0;

		// Check for no trade
		for (int i = 0; i < itemCount; i++) {
			ManagedReference<SceneObject*> object = sceneObject->getContainerObject(i);
			if (object != NULL) {
				if (object->isNoTrade()) {
					player->sendSystemMessage(" \\#ff0000[ATTENTION] \\#ffffffThe Sarlacc Trash Can tried to consume an item marked as \\#ff982bNo Trade\\#ffffff. The item was regurgitated and not destroyed, please remove the item.");
					return;
				}
			}
			if (i > 15) {
				player->sendSystemMessage(" \\#f6d53b[WARNING] \\#ffffffThe Sarlacc Trash Can tried to consume more then 15 items. Nothing was destroyed.");
				return;
			}
		}

		while (sceneObject->getContainerObjectsSize() > 0) { // Delete items
			ManagedReference<SceneObject*> object = sceneObject->getContainerObject(0);
			if (object != NULL) {
				Locker trashCanObjectLocker(object);
				object->destroyObjectFromWorld(true);
				deletedItems++;
				if (object->isPersistent())
					object->destroyObjectFromDatabase(true);
			} else {
				player->sendSystemMessage("The Sarlacc Trash Can has indigestion. Please report this message.");
				return;
			}
		}

		if(!sceneObject->isTangibleObject())
			return;

		TangibleObject* tano = cast<TangibleObject*>(sceneObject.get());

		Locker locker(tano);
		int useCount = tano->getUseCount();
		if (useCount == 1)
			tano->setUseCount(0, true); // Do not destroy if use count is 1
		else
			tano->decreaseUseCount();

		useCount--;

		player->sendSystemMessage("The Sarlacc Trash Can consumed \\#ffd27f" + String::valueOf(deletedItems) + "\\#ffffff item(s). \\#ffe254" + String::valueOf(useCount) + " \\#ffffffcharge(s) remain.");
	}
};

#endif /* SARLACCTRASHCANSUICALLBACK_H_ */
