/*
 * DataDiskGetPageSuiCallback.h
 *
 *  Created on: 30/6/2019
 *      Author: Tyclo
 */

#ifndef DATADISKGETPAGESUICALLBACK_H_
#define DATADISKGETPAGESUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"

class DataDiskGetPageSuiCallback : public SuiCallback {
	ManagedReference<TangibleObject*> tano;

	int curPage;
	int pagesMax;

public:
	DataDiskGetPageSuiCallback(ZoneServer* server, TangibleObject* tano, int curPage, int pagesMax)
			: SuiCallback(server) {

		this->tano = tano;
		this->curPage = curPage;
		this->pagesMax = pagesMax;
	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {

		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed || player == NULL)
			return;

		curPage += 1;

		if (curPage > pagesMax || curPage < 0)
			return;

		String templateName = tano->getObjectTemplate()->getTemplateFileName();

		// Template is string name
		String title = "@encoded_disk/disk_data:" + templateName + "_name (" + String::valueOf(curPage + 1) + "/" + String::valueOf(pagesMax + 1) + ")";
		String text = "";
		if (curPage != 0)
			text = "@encoded_disk/disk_data:"  + templateName + "_message_" + String::valueOf(curPage);
		else
			text = "@encoded_disk/disk_data:"  + templateName + "_message";

		// Build and send SUI to player
		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::DATADISKMESSAGE);
		box->setPromptTitle(title);
		box->setPromptText(text);
		box->setUsingObject(player);
		box->setForceCloseDisabled();

		if (curPage < pagesMax)
			box->setOkButton(true, "@ui:next");
		else
			box->setOkButton(true, "@ui:close");

		box->setCallback(new DataDiskGetPageSuiCallback(player->getZoneServer(), tano, curPage, pagesMax));

		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());
	}
};

#endif /* DATADISKGETPAGESUICALLBACK_H_ */
