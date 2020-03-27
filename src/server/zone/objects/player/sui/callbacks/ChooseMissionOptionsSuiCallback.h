/*
 * ChooseMissionDifficultySuiCallback.h
 *
 *  Created on: Mar 17, 2020
 *      Author: Mindsoft
 * 		Inspired By: Tarkins Revenge (Forever in Appreciation)
 */

#ifndef CHOOSE_MISSION_OPTIONS_SUI_CALLBACK_H_
#define CHOOSE_MISSION_OPTIONS_SUI_CALLBACK_H_

#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

class ChooseMissionOptionsSuiCallback : public SuiCallback {
	
public:
	ChooseMissionOptionsSuiCallback(ZoneServer* serv) : SuiCallback(serv) {
	}

	void run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		if((eventIndex == 1) || !creature->isPlayerCreature()) {
			return;
		}

		if(sui->getWindowType() == SuiWindowType::MISSION_CHOOSE_DIRECTION)
			handleSelectDirection(creature, sui, args);

		if(sui->getWindowType() == SuiWindowType::MISSION_CHOOSE_LEVEL)
			handleSelectLevel(creature, sui, args);
	}

	void handleSelectDirection(CreatureObject* creature, SuiBox* sui, Vector<UnicodeString>* args) {
		SuiListBox* listBox = cast<SuiListBox*>(sui);
		int index = Integer::valueOf(args->get(0).toString());

		if (listBox == NULL || (index < 0 || index > 8)) {
			return;
		}

		PlayerObject* ghost = creature->getPlayerObject();
		if (ghost == NULL) {
			return;
		}

		String choice = String::valueOf(listBox->getMenuObjectID(index));
		ghost->setScreenPlayData("mission_terminal_choice", "directionChoice", choice);
	}


	void handleSelectLevel(CreatureObject* creature, SuiBox* sui, Vector<UnicodeString>* args) {
		SuiListBox* listBox = cast<SuiListBox*>(sui);
		int index = Integer::valueOf(args->get(0).toString());

		if (listBox == NULL || (index < 0 || index > 9)) {
			return;
		}

		PlayerObject* ghost = creature->getPlayerObject();
		if (ghost == NULL) {
			return;
		}

		String choice = String::valueOf(listBox->getMenuObjectID(index));
		ghost->setScreenPlayData("mission_terminal_choice", "levelChoice", choice);
	}
};

#endif /* CHOOSE_MISSION_OPTIONS_SUI_CALLBACK_H_ */
