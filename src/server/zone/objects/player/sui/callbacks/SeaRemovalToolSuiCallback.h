/*
 * SeaRemovalToolSuiCallback.h
 *
 *  Created on: March 5, 2020
 *      Author: Mindsoft
 */

#ifndef SEAREMOVALTOOLSUICALLBACK_H_
#define SEAREMOVALTOOLSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
//#include "server/zone/objects/scene/SceneObject.h"

class SeaRemovalToolSuiCallback : public SuiCallback {


public:
	SeaRemovalToolSuiCallback(ZoneServer* serv) : SuiCallback(serv) {

	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		SuiListBox* listBox = cast<SuiListBox*>(sui);
		int index = Integer::valueOf(args->get(0).toString());

		if(cancelPressed || listBox == NULL || (index < 0)) {
			return;
		}

		ManagedReference<SceneObject*> sceneObject = sui->getUsingObject().get();
		if (sceneObject == NULL)
			return;

		TangibleObject* tano = cast<TangibleObject*>( sceneObject.get());
		WearableObject* wearable = cast<WearableObject*>(tano);
		ManagedReference<SceneObject*> sea = NULL;
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");
		if (wearable != NULL && inventory != NULL) { //safety Checks

			if (wearable->hasSeaRemovalTool(player, false) ==  false){ //They need the tool
				player->sendSystemMessage("This requires a specialzied skill enhancing attachment removal tool.");
				return;
			}

			if (wearable->isWearableObject() || wearable->isArmorObject()){
				if (wearable->isEquipped()){
					player->sendSystemMessage("You must first un-equip the item.");
					return;
				}

				VectorMap<String, int>* mods = wearable->getWearableSkillMods();
				int modCount = mods->size();
				if (modCount > 0) {	//If the item has no mods we're done
					ManagedReference<LootManager*> lootManager = player->getZoneServer()->getLootManager();
					LootGroupMap* lootGroupMap = LootGroupMap::instance();
					Reference<LootItemTemplate*> itemTemplate = NULL;
					String objectTemplate = "";
					objectTemplate = sceneObject->getObjectTemplate()->getFullTemplateString();

					// Get Wearable Type and Mods
					int modTotal = 0;
					if (wearable->isArmorObject() ||
						objectTemplate == "object/tangible/wearables/armor/padded/armor_padded_s01_belt.iff"  ||
						objectTemplate == "object/tangible/wearables/armor/zam/armor_zam_wesell_belt.iff" ||
						objectTemplate == "object/tangible/wearables/armor/stormtrooper/armor_stormtrooper_utility_belt.iff") {
						itemTemplate = lootGroupMap->getLootItemTemplate("attachment_armor");
						if (player->hasSkill("crafting_armorsmith_master")) // + 30
							modTotal += 30;
						else if (player->hasSkill("crafting_armorsmith_novice")) // + 15
							modTotal += 15;

						modTotal += player->getSkillMod("armor_repair"); // max 25

					} else {
						itemTemplate = lootGroupMap->getLootItemTemplate("attachment_clothing");
						if (player->hasSkill("crafting_tailor_master")) // + 30
							modTotal += 30;
						else if (player->hasSkill("crafting_tailor_novice")) // + 15
							modTotal += 15;

						modTotal += player->getSkillMod("clothing_repair"); // max 25

					}

					if (lootGroupMap == NULL){
						return;
					}

					modTotal += player->getSkillMod("crafting_repair"); // max ?
					modTotal += player->getSkillMod("force_repair_bonus"); // max 20
					modTotal += System::random(player->getSkillMod("force_luck")); // max 4
					//printf("MODFIER TOTAL:%d\n",modTotal);

					int chance = 0;
					int difficulty = 0;
					int modifiers = (modTotal / 79.0f * 10.0f); // (0-10) 0=NO MODS  ,  10=FULL MODS

					if (modCount == 2)
						difficulty = 40;
					else if (modCount == 3)
						difficulty = 30;
					else if (modCount == 4)
						difficulty = 15;

					for (int i=0;i<modCount;i++){//Remove skill mods from item and create tapes
						if (i == index) {
							chance = 100;
							difficulty -= 15;
						} else {
							chance = System::random(95-difficulty);
						}

						//printf("Mods:%d Chance:%d Difficulty:%d Preserve?:%s\n",modifiers,chance,difficulty,((chance + modifiers) >= difficulty)? "true" : "false");
						if ((chance + modifiers) >= difficulty) {
							String modKey = mods->elementAt(i).getKey();
							if (!isRestrictedSEA(modKey)) {
								difficulty += 15;
								sea = lootManager->createLootAttachment(itemTemplate,modKey, mods->elementAt(i).getValue());
								if (sea != NULL){
									Attachment* attachment = cast<Attachment*>(sea.get());
									if (attachment != NULL){
										Locker objLocker(attachment);
										if (inventory->transferObject(sea, -1, true, true)) { //Transfer tape to player inventory
											inventory->broadcastObject(sea, true);
										} else {
											sea->destroyObjectFromDatabase(true);
											return;
										}
									}
								}
							}
						}
					}

					//Destroy item now that tapes have been generated
					if (wearable->hasSeaRemovalTool(player,true) ==  true) {
						player->sendSystemMessage("Your SEA Tool has been consumed in the process.");
						wearable->destroyObjectFromWorld(true);
						wearable->destroyObjectFromDatabase(true);
					}
				}
			}
		}
	}

	bool isRestrictedSEA(String modkey) {
		bool result = true;
		if (modkey == "combat_bleeding_defense"){}
		else if (modkey == "healing_dance_wound"){}
		else if (modkey == "healing_injury_treatment"){}
		else if (modkey ==  "healing_music_wound"){}
		else if (modkey ==  "healing_wound_treatment"){}
		else if (modkey ==  "stun_defense"){}
		else if (modkey ==  "tame_aggro"){}
		else if (modkey ==  "tame_non_aggro"){}
		else if (modkey ==  "warcry"){}
		else if (modkey ==  "healing_ability"){}
		else if (modkey ==  "healing_wound_speed"){}
		else if (modkey ==  "healing_dance_shock"){}
		else if (modkey ==  "healing_music_shock"){}
		else if (modkey ==  "creature_harvesting"){}
		else if (modkey ==  "camp"){}
		else if (modkey ==  "healing_range"){}
		else
			result = false;

		return result;
	}
};

#endif /* SEAREMOVALTOOLSUICALLBACK_H_ */
