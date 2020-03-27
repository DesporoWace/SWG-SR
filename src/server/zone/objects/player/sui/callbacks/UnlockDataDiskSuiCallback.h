/*
 * UnlockDataDiskSuiCallback.h
 *
 *  Created on: 30/6/2019
 *      Author: Tyclo
 */

#ifndef UNLOCKDATADISKSUICALLBACK_H_
#define UNLOCKDATADISKSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/components/generic/DataDiskDataComponent.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"

class UnlockDataDiskSuiCallback : public SuiCallback {
	ManagedReference<DataDiskDataComponent*> dataDisk;
	ManagedReference<TangibleObject*> tano;

	int unlockCode;

public:
	UnlockDataDiskSuiCallback(ZoneServer* server, DataDiskDataComponent* dataDiskObj, TangibleObject* tanoObj, int code)
			: SuiCallback(server) {
		this->dataDisk = dataDiskObj;
		this->tano = tanoObj;
		this->unlockCode = code;
	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed || player == NULL)
			return;

		if (args->size() < 0)
			return;

		int codeInput = Integer::valueOf(args->get(0).toString());

		if (codeInput == 0 || codeInput > 999999) {
			player->sendSystemMessage("Invalid code.");
			return;
		}

		if (unlockCode != codeInput) {
			player->sendSystemMessage("Incorrect code. Please try again.");
			return;
		}

		// Stop asking for code
		dataDisk->setUnlockCode(1);

		String templateName = tano->getObjectTemplate()->getTemplateFileName();

		// Template is string name
		String title = "@encoded_disk/disk_data:" + templateName + "_name";
		String text = "@encoded_disk/disk_data:"  + templateName + "_message";

		// Build and send SUI to player
		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::DATADISKMESSAGE);
		box->setPromptTitle(title);
		box->setPromptText(text);
		box->setUsingObject(player);
		box->setForceCloseDisabled();

		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());
	}
};

#endif /* UNLOCKDATADISKSUICALLBACK_H_ */
