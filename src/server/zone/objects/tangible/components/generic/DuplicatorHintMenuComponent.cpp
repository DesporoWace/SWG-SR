/*
 * DuplicatorHintMenuComponent
 *
 *  Created on: 02/22/2020
 *      Author: Tyclo
 */

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "DuplicatorHintMenuComponent.h"
#include "server/zone/objects/tangible/components/generic/DuplicatorHintDataComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/managers/stringid/StringIdManager.h"

void DuplicatorHintMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject,
	ObjectMenuResponse* menuResponse, CreatureObject* player) const {

	if (!sceneObject->isTangibleObject())
		return;

	TangibleObject* tano = cast<TangibleObject*>(sceneObject);
	if (tano == NULL)
		return;

	DataObjectComponent* data = tano->getDataObjectComponent()->get();
	if (data == NULL)
		return;

	DuplicatorHintDataComponent* dhData = cast<DuplicatorHintDataComponent*>(data);
	if (dhData == NULL || !dhData->isDuplicatorHintData())
		return;

	int id = dhData->getId();
	String radialText = "Read Disk";

	if (id == -1) {
		radialText = "Decrypt Disk";
		if(tano->isASubChildOf(player) && player->hasSkill("combat_smuggler_slicing_02"))
			menuResponse->addRadialMenuItem(69, 3, "Slice Disk");
	}

	if (sceneObject->checkContainerPermission(player, ContainerPermissions::MOVECONTAINER && sceneObject->getParent().get() != NULL &&
		sceneObject->getParent().get()->checkContainerPermission(player, ContainerPermissions::MOVEOUT)))
			menuResponse->addRadialMenuItem(20, 3, radialText);

	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);
}

int DuplicatorHintMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject,
		CreatureObject* player, byte selectedID) const {

	if (selectedID != 20 && selectedID != 69) {
		return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
	}

	bool isSlicing = false;

	if (selectedID == 69) {
		if (player->hasSkill("combat_smuggler_slicing_02")) {
			isSlicing = true;
		} else {
			player->sendSystemMessage("You lack the required skills to slice this item.");
			return 0;
		}
	}

	if(!sceneObject->isTangibleObject())
		return 0;

	ManagedReference<TangibleObject*> tano = cast<TangibleObject*>(sceneObject);

	if (!player->isPlayerCreature())
		return 0;

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == NULL)
		return 0;

	if (ghost->hasSuiBoxWindowType(SuiWindowType::DATADISKMESSAGE))
		return 0;

	DataObjectComponent* data = tano->getDataObjectComponent()->get();
	if (data == NULL) {
		player->sendSystemMessage("Unable to validate this Duplicator Data Disk.");
		return 0;
	}

	DuplicatorHintDataComponent* dhData = cast<DuplicatorHintDataComponent*>(data);
	if (dhData == NULL || !dhData->isDuplicatorHintData()) {
		player->sendSystemMessage("This is not a valid Duplicator Data Disk.");
		return 0;
	}

	int id = dhData->getId();
	int group = -1;
	bool setName = false;

	if (!tano->isASubChildOf(player) && id == -1) {
		player->sendSystemMessage("A Duplicator Data Disk can only be decrypted while in your inventory.");
		return 0;
	}

	Lua* lua = new Lua();
	lua->init();

	if (!lua->runFile("scripts/managers/duplicator_manager.lua")) { // Get file containing combinations list
		delete lua;
		return 0;
	}

	if (id == -1) {
		LuaObject duplicatorComboListSet = lua->getGlobalObject("duplicatorCombinationsList");

		if (!duplicatorComboListSet.isValidTable() && duplicatorComboListSet.getTableSize() > 0)
			return 0;

		group = getGroupId(isSlicing);
		LuaObject groupSet = duplicatorComboListSet.getObjectAt(group);

		if (group != -1 && !groupSet.isValidTable()) {
			player->sendSystemMessage("The Duplicator Data Disk was unable to set a Duplicator Combination.");
			return 0;
		}

		dhData->setGroup(group);

		id = System::random(groupSet.getTableSize() - 1) + 1;
		dhData->setId(id);
		setName = true;
	} else {
		if (isSlicing) {
			player->sendSystemMessage("This Duplicator Data Disk has already been sliced.");
			return 0;
		}
		group = dhData->getGroup();
	}

	if (group == -1) {
		player->sendSystemMessage("The Duplicator Data Disk was unable to validate a Duplicator Combination.");
		return 0;
	}

	LuaObject duplicatorComboList = lua->getGlobalObject("duplicatorCombinationsList");

	if (!duplicatorComboList.isValidTable() && duplicatorComboList.getTableSize() > 0)
		return 0;

	LuaObject groupList = duplicatorComboList.getObjectAt(group);
	LuaObject combination = groupList.getObjectAt(id);

	if (!combination.isValidTable()) {
		player->sendSystemMessage("The Duplicator Data Disk was unable to generate a Duplicator Combination.");
		return 0;
	}

	String comboItem = "";
	String itemName = "";
	StringBuffer message;
	String reward = "";

	StringIdManager* sidman = StringIdManager::instance();
	reward = combination.getStringField("reward");
	SharedObjectTemplate* templateData = TemplateManager::instance()->getTemplate(reward.hashCode());
	reward = templateData->getObjectName();
	reward = sidman->getStringId(reward).toString();
	message << "\\#fff1bcReward: \\#ffffff" << reward << endl;

	if (setName)
		sceneObject->setCustomObjectName(reward + " (Duplicator Combination)", true);

	LuaObject combo = combination.getObjectField("combo");
	for (int x = 1; x <= combo.getTableSize(); ++x) {
		comboItem = combo.getStringAt(x);
		comboItem = comboItem.subString(comboItem.length() - 6, comboItem.length() - 4);
		itemName = sidman->getStringId(("@duplicator_loot:duplicator_loot_" + comboItem + "_n").hashCode()).toString();
		comboItem = "Module\\#" + getRarityColor(comboItem.subString(0, 1));
		itemName = itemName.replaceFirst("Module",comboItem);
		message << "\t\\#cee5e5" << String::valueOf(x) << ") \\#eeeeee" << itemName << endl;
	}

	// Build and send SUI to player
	ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::DATADISKMESSAGE);
	box->setPromptTitle(reward);
	box->setPromptText(message.toString());
	box->setUsingObject(player);
	box->setForceCloseDisabled();

	player->getPlayerObject()->addSuiBox(box);
	player->sendMessage(box->generateMessage());

	delete lua;

	return 0;
}

String DuplicatorHintMenuComponent::getRarityColor(String rarity) const {
	if (rarity == "0")
		return "f8edcf";
	else if (rarity == "1")
		return "e3def1";
	else if (rarity == "2")
		return "d8e4f7";
	else if (rarity == "3")
		return "e2f6d5";
	else
		return "cccccc";
}

int DuplicatorHintMenuComponent::getGroupId(bool isSlicing) const {
	int roll = System::random(10);

	if (isSlicing)
		roll += 2;

	if (roll <= 5) {
		return 1; // common
	} else if (roll >= 6 && roll <= 8) {
		return 2; // uncommon
	} else {
		return 3; // rare
	}
}
