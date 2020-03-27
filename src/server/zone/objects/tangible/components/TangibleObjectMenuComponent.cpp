/*
 * TangibleObjectMenuComponent.cpp
 *
 *  Created on: 26/05/2011
 *      Author: victor
 */

#include "TangibleObjectMenuComponent.h"
#include "server/zone/objects/player/sessions/SlicingSession.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/managers/loot/LootGroupMap.h"

#include "server/zone/objects/player/sui/callbacks/SeaRemovalToolSuiCallback.h"


void TangibleObjectMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	ObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	uint32 gameObjectType = sceneObject->getGameObjectType();

	if (!sceneObject->isTangibleObject())
		return;

	TangibleObject* tano = cast<TangibleObject*>( sceneObject);

	// Figure out what the object is and if its able to be Sliced.
	if(tano->isSliceable() && !tano->isSecurityTerminal()) { // Check to see if the player has the correct skill level

		bool hasSkill = true;
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if ((gameObjectType == SceneObjectType::PLAYERLOOTCRATE) && !player->hasSkill("combat_smuggler_novice"))
			hasSkill = false;
		else if (sceneObject->isContainerObject())
			hasSkill = false; // Let the container handle our slice menu
		else if (sceneObject->isMissionTerminal() && !player->hasSkill("combat_smuggler_slicing_01"))
			hasSkill = false;
		else if (sceneObject->isWeaponObject() && (!inventory->hasObjectInContainer(sceneObject->getObjectID()) || !player->hasSkill("combat_smuggler_slicing_02")))
			hasSkill = false;
		else if (sceneObject->isArmorObject() && (!inventory->hasObjectInContainer(sceneObject->getObjectID()) || !player->hasSkill("combat_smuggler_slicing_03")))
			hasSkill = false;

		if(hasSkill)
			menuResponse->addRadialMenuItem(69, 3, "@slicing/slicing:slice"); // Slice
	}

	if(player->getPlayerObject() != NULL && player->getPlayerObject()->isPrivileged()) {
		/// Viewing components used to craft item, for admins
		ManagedReference<SceneObject*> container = tano->getSlottedObject("crafted_components");
		if(container != NULL) {

			if(container->getContainerObjectsSize() > 0) {

				SceneObject* satchel = container->getContainerObject(0);

				if(satchel != NULL && satchel->getContainerObjectsSize() > 0) {
					menuResponse->addRadialMenuItem(79, 3, "@ui_radial:ship_manage_components"); // View Components
				}
			}
		}
	}

	WearableObject* wearable = cast<WearableObject*>(tano);
	if (wearable != NULL)
	if (wearable->hasSeaRemovalTool(player, false) ==  true)
	if (wearable->isWearableObject() || wearable->isArmorObject()){
		VectorMap<String, int>* mods = wearable->getWearableSkillMods();
			if (mods->size() > 0)
				menuResponse->addRadialMenuItem(89,3,"Extract Skill Mods");
		}


	ManagedReference<SceneObject*> parent = tano->getParent().get();
	if (parent != NULL && parent->getGameObjectType() == SceneObjectType::STATICLOOTCONTAINER) {
		menuResponse->addRadialMenuItem(10, 3, "@ui_radial:item_pickup"); //Pick up
	}
}

int TangibleObjectMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!sceneObject->isTangibleObject())
		return 0;

	TangibleObject* tano = cast<TangibleObject*>( sceneObject);


	if (selectedID == 69 && player->hasSkill("combat_smuggler_novice") ) { // Slice [PlayerLootCrate]
		if (player->containsActiveSession(SessionFacadeType::SLICING)) {
			player->sendSystemMessage("@slicing/slicing:already_slicing");
			return 0;
		}

		//Create Session
		ManagedReference<SlicingSession*> session = new SlicingSession(player);
		session->initalizeSlicingMenu(player, tano);

		return 0;
	} else if (selectedID == 79) { // See components (admin)
		if(player->getPlayerObject() != NULL && player->getPlayerObject()->isPrivileged()) {

			SceneObject* container = tano->getSlottedObject("crafted_components");
			if(container != NULL) {

				if(container->getContainerObjectsSize() > 0) {

					SceneObject* satchel = container->getContainerObject(0);

					if(satchel != NULL) {

						satchel->sendWithoutContainerObjectsTo(player);
						satchel->openContainerTo(player);

					} else {
						player->sendSystemMessage("There is no satchel this container");
					}
				} else {
					player->sendSystemMessage("There are no items in this container");
				}
			} else {
				player->sendSystemMessage("There is no component container in this object");
			}
		}

		return 0;
	} else if (selectedID == 89) { //Remove SEA Mods from wearable

		WearableObject* wearable = cast<WearableObject*>(tano);
		ManagedReference<SceneObject*> sea = NULL;
		bool convertedMods = false;
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");
		if (wearable != NULL && inventory != NULL) { //safety Checks

			if (wearable->hasSeaRemovalTool(player, false) ==  false){ //They need the tool
				player->sendSystemMessage("This requires a specialzied skill enhancing attachment removal tool.");
				return 0;
			}

			if (wearable->isWearableObject() || wearable->isArmorObject()){
				if (wearable->isEquipped()){
					player->sendSystemMessage("You must first un-equip the item.");
					return 0;
				}

				VectorMap<String, int>* mods = wearable->getWearableSkillMods();
				int modCount = mods->size();
				if (modCount > 0) {	//If the item has no mods we're done
					ManagedReference<LootManager*> lootManager = player->getZoneServer()->getLootManager();
					int i,j;
					LootGroupMap* lootGroupMap = LootGroupMap::instance();
					Reference<LootItemTemplate*> itemTemplate = NULL;
					String objectTemplate = "";
					objectTemplate = sceneObject->getObjectTemplate()->getFullTemplateString();

					//error("ObjectTempate = " + objectTemplate);
					if (wearable->isArmorObject() ||
						objectTemplate == "object/tangible/wearables/armor/padded/armor_padded_s01_belt.iff"  ||
						objectTemplate == "object/tangible/wearables/armor/zam/armor_zam_wesell_belt.iff" ||
						objectTemplate == "object/tangible/wearables/armor/stormtrooper/armor_stormtrooper_utility_belt.iff") {
						//error("Detected as armor or belt");
						itemTemplate = lootGroupMap->getLootItemTemplate("attachment_armor");
					} else {
						//error("detect as clothing");
						itemTemplate = lootGroupMap->getLootItemTemplate("attachment_clothing");
					}

					if (lootGroupMap == NULL){
						error("Invalid loot template");
						return 0;
					}

					String modMsg = "Select a Skill Mod to Retrieve:\n -This Skill mod will be returned and item is destroyed.\n";
					if (modCount > 1) {
						modMsg += "\n Difficulty Rolls:\n";
						if (modCount == 2)
							modMsg += "  2nd:(40)\n";
						else if (modCount == 3)
							modMsg += "  2nd:(30)  3rd:(45)\n";
						else
							modMsg += "  2nd:(15)  3rd:(30)  4th:(45)\n";
					}

					// Build and Send Player SEA Remove: Select Attachment (Window)
					ManagedReference<SuiListBox*> list = new SuiListBox(player, SuiWindowType::NONE, SuiListBox::HANDLETWOBUTTON);//0x00);
					SeaRemovalToolSuiCallback* seaCallback = new SeaRemovalToolSuiCallback(player->getZoneServer());
					list->setPromptTitle("SEA Removal: Mod Select");
					list->setCallback(seaCallback);
					list->setUsingObject(wearable);
					list->setOkButton(true, "Select");
					list->setCancelButton(true, "@cancel");
					list->setForceCloseDistance(0.f);

					bool hasRestricted = false;
					StringIdManager* sidman = StringIdManager::instance();
					for (i=0;i<modCount;i++){//Remove skill mods from item and create tapes
						String modKey = mods->elementAt(i).getKey();
						String modName = sidman->getStringId("@stat_n:" + modKey).toString();
						if (seaCallback->isRestrictedSEA(modKey)) {
							player->sendSystemMessage(modName + ": [RESTRICTED]"); // Alert player mod is restricted
							if (!hasRestricted) {
								hasRestricted = true;
								modMsg += "\nRestricted Mods:\n";
							}
							modMsg += "  " + modName + " ( " + mods->elementAt(i).getValue() + " )\n";
						} else
							list->addMenuItem(modName + ": ( " + mods->elementAt(i).getValue() + " )", i); // add mod to list
					}

					list->setPromptText(modMsg);
					player->getPlayerObject()->addSuiBox(list); // Send window to player for callback
					player->sendMessage(list->generateMessage()); // show player
				}
			}
		}

	return 0;
	} else
		return ObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);

}
