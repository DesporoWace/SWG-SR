/*
 * MissionTerminalImplementation.cpp
 *
 *  Created on: 03/05/11
 *      Author: polonel
 */

#include "server/zone/objects/tangible/terminal/mission/MissionTerminal.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/managers/city/CityManager.h"
#include "server/zone/managers/city/CityRemoveAmenityTask.h"
#include "server/zone/objects/player/sessions/SlicingSession.h"

#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/sui/callbacks/ChooseMissionOptionsSuiCallback.h"

void MissionTerminalImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	TerminalImplementation::fillObjectMenuResponse(menuResponse, player);

	ManagedReference<CityRegion*> city = player->getCityRegion().get();

	if (city != NULL && city->isMayor(player->getObjectID()) && getParent().get() == NULL) {

		menuResponse->addRadialMenuItem(72, 3, "@city/city:mt_remove"); // Remove

		menuResponse->addRadialMenuItem(73, 3, "@city/city:align"); // Align
		menuResponse->addRadialMenuItemToRadialID(73, 74, 3, "@city/city:north"); // North
		menuResponse->addRadialMenuItemToRadialID(73, 75, 3, "@city/city:east"); // East
		menuResponse->addRadialMenuItemToRadialID(73, 76, 3, "@city/city:south"); // South
		menuResponse->addRadialMenuItemToRadialID(73, 77, 3, "@city/city:west"); // West
	}

	if (isGeneralTerminal()) { // Mindsoft Added , Inspired By: Tarkins Revenge
		menuResponse->addRadialMenuItem(101, 3, "Choose Mission Level");
		menuResponse->addRadialMenuItem(102, 3, "Choose Mission Direction");
	}
}

int MissionTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	ManagedReference<CityRegion*> city = player->getCityRegion().get();

	if ((selectedID == 101) || (selectedID == 102)) { // Mindsoft Added , Inspired By: Tarkins Revenge
		PlayerObject* ghost = player->getPlayerObject();
		if (ghost == nullptr)
			return 0;

		String msg;
		int playerLevel;
		if (player->isGrouped()) {
			playerLevel = player->getGroup()->getGroupLevel();
			msg = "\n\nGroup Level: " + getLevelName(playerLevel);
		} else {
			playerLevel = player->getZoneServer()->getPlayerManager()->calculatePlayerLevel(player);
			msg = "\n\nPlayer Level: " + getLevelName(playerLevel);
		}

		String levelChoice = ghost->getScreenPlayData("mission_terminal_choice", "levelChoice");
		msg += "\n\nSelected Difficutly: " + getLevelName(Integer::valueOf(levelChoice));

		String directionChoice = ghost->getScreenPlayData("mission_terminal_choice", "directionChoice");
		msg += "\nSelected Direction: " + getDirectionName(Integer::valueOf(directionChoice));

		ManagedReference<SuiListBox*> suiBox;
		
		if (selectedID == 101) {
			msg = "\tSelect the desired mission difficulty level." + msg;
			suiBox = new SuiListBox(player, SuiWindowType::MISSION_CHOOSE_LEVEL, SuiListBox::HANDLETWOBUTTON);
			suiBox->setPromptTitle("Mission Terminal: Select Level");
			suiBox->setPromptText(msg);
			suiBox->addMenuItem("Default (Random)", 0);
			suiBox->addMenuItem("Easy", 1);
			if (playerLevel > 1)
				suiBox->addMenuItem("Medium 1", 2);
			if (playerLevel > 11)
				suiBox->addMenuItem("Medium 2", 12);
			if (playerLevel > 24)
				suiBox->addMenuItem("Medium 3", 25);
			if (playerLevel > 34)
				suiBox->addMenuItem("High 1", 35);
			if (playerLevel > 44)
				suiBox->addMenuItem("High 2", 45);
			if (playerLevel > 59)
				suiBox->addMenuItem("High 3", 60);
			if (playerLevel > 134)
				suiBox->addMenuItem("Hard", 135);
			if (playerLevel > 199)
				suiBox->addMenuItem("Hardest", 200);

			suiBox->setCallback(new ChooseMissionOptionsSuiCallback(player->getZoneServer()));
			
		} else if (selectedID == 102) {
			msg = "\tSelect the desired mission direction." + msg;
			suiBox = new SuiListBox(player, SuiWindowType::MISSION_CHOOSE_DIRECTION, SuiListBox::HANDLETWOBUTTON);
			suiBox->setPromptTitle("Mission Terminal: Select Direction");
			suiBox->setPromptText(msg);
			suiBox->addMenuItem("Default (Random)", 0);
			suiBox->addMenuItem("North", 360);
			suiBox->addMenuItem("North-East", 315);
			suiBox->addMenuItem("East", 270);
			suiBox->addMenuItem("South-East", 225);
			suiBox->addMenuItem("South", 180);
			suiBox->addMenuItem("South-West", 135);
			suiBox->addMenuItem("West", 90);
			suiBox->addMenuItem("North-West", 45);
			suiBox->setCallback(new ChooseMissionOptionsSuiCallback(player->getZoneServer()));
		}

		suiBox->setUsingObject(_this.getReferenceUnsafeStaticCast());
		suiBox->setOkButton(true, "@ok");
		suiBox->setCancelButton(true, "@cancel");
		ghost->addSuiBox(suiBox); // Send object to player for callback data
		player->sendMessage(suiBox->generateMessage()); // Send sui window to player UI

	} else if (selectedID == 69 && player->hasSkill("combat_smuggler_slicing_01")) {
		if (isBountyTerminal())
			return 0;

		if (city != NULL && !city->isClientRegion() && city->isBanned(player->getObjectID())) {
			player->sendSystemMessage("@city/city:banned_services"); // You are banned from using this city's services.
			return 0;
		}

		if (player->containsActiveSession(SessionFacadeType::SLICING)) {
			player->sendSystemMessage("@slicing/slicing:already_slicing");
			return 0;
		}

		if (!player->checkCooldownRecovery("slicing.terminal")) {
			StringIdChatParameter message;
			message.setStringId("@slicing/slicing:not_yet"); // You will be able to hack the network again in %DI seconds.
			message.setDI(player->getCooldownTime("slicing.terminal")->getTime() - Time().getTime());
			player->sendSystemMessage(message);
			return 0;
		}

		//Create Session
		ManagedReference<SlicingSession*> session = new SlicingSession(player);
		session->initalizeSlicingMenu(player, _this.getReferenceUnsafeStaticCast());

		return 0;

	} else if (selectedID == 72) {

		if (city != NULL && city->isMayor(player->getObjectID())) {
			CityRemoveAmenityTask* task = new CityRemoveAmenityTask(_this.getReferenceUnsafeStaticCast(), city);
			task->execute();

			player->sendSystemMessage("@city/city:mt_removed"); // The object has been removed from the city.
		}

		return 0;

	} else if (selectedID == 74 || selectedID == 75 || selectedID == 76 || selectedID == 77) {

		CityManager* cityManager = getZoneServer()->getCityManager();
		cityManager->alignAmenity(city, player, _this.getReferenceUnsafeStaticCast(), selectedID - 74);

		return 0;
	}

	return TangibleObjectImplementation::handleObjectMenuSelect(player, selectedID);
}

String MissionTerminalImplementation::getTerminalName() {
	String name = "@terminal_name:terminal_mission";

	if (terminalType == "artisan" || terminalType == "entertainer" || terminalType == "bounty" || terminalType == "imperial" || terminalType == "rebel" || terminalType == "scout")
		name = name + "_" + terminalType;

	return name;
}

// @dir degrees return compass direction string
String MissionTerminalImplementation::getDirectionName(int dir) {
		String direction;
		switch(dir){
			case 0: direction = "Default (Random)";
				break;
			case 360: direction = "North";
				break;
			case 315: direction = "North-East";
				break;
			case 270: direction = "East";
				break;
			case 225: direction = "South-East";
				break;
			case 180: direction = "South";
				break;
			case 135: direction = "South-West";
				break;
			case 90: direction = "West";
				break;
			case 45: direction = "North-West";
				break;
		}

		return direction;
	}

// @lvl difficulty selection return compass difficulty string
String MissionTerminalImplementation::getLevelName(int lvl) {
	String name;
	if (lvl < 1)
		name = "Default (Random)";
	else if (lvl > 199)
		name = "Hardest";
	else if (lvl > 134)
		name = "Hard";
	else if (lvl > 59)
		name = "High 3";
	else if (lvl > 44)
		name = "High 2";
	else if (lvl > 34)
		name = "High 1";
	else if (lvl > 24)
		name = "Medium 3";
	else if (lvl > 11)
		name = "Medium 2";
	else if (lvl > 1)
		name = "Medium 1";
	else
		name = "Easy";

	return name;
}
