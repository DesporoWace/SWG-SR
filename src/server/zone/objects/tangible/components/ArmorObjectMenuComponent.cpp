/*
 * ArmorObjectMenuComponent.cpp
 *
 *  Created on: 2016 10 13
 *      Author: Aaron Tate (Zero Divide)
 *		Credits: bluree, TA & Valk
 */

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/player/sui/colorbox/SuiColorBox.h"
#include "ArmorObjectMenuComponent.h"
#include "server/zone/objects/scene/components/ObjectMenuComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/ColorArmorSuiCallback.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "templates/customization/AssetCustomizationManagerTemplate.h"

void ArmorObjectMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {

	if (!sceneObject->isWearableObject())
		return;

	ManagedReference<SceneObject*> parent = sceneObject->getParent().get();

	if (parent != NULL && parent->isCellObject()) {
		ManagedReference<SceneObject*> obj = parent->getParent().get();

		if (obj != NULL && obj->isBuildingObject()) {
			ManagedReference<BuildingObject*> buio = cast<BuildingObject*>(obj.get());

			if (!buio->isOnAdminList(player))
				return;
		}
	}
	else
	{
		if (!sceneObject->isASubChildOf(player))

			return;
	}

	String text = "Item Color Change";
	menuResponse->addRadialMenuItem(80, 3, text);
	menuResponse->addRadialMenuItemToRadialID(80, 81, 3, "Primary");
	menuResponse->addRadialMenuItemToRadialID(80, 82, 3, "Secondary");
	
    WearableObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player); 	
}

int ArmorObjectMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!sceneObject->isWearableObject())
		return 0;

	if (!sceneObject->isASubChildOf(player))
		return 0;
	
	if (selectedID == 81 || selectedID == 82) {
		
		ManagedReference<SceneObject*> parent = sceneObject->getParent().get();
		if (parent == NULL && parent->isPlayerCreature()){
			player->sendSystemMessage("@armor_rehue:equipped");
			return 0;
		}

		// Handle color change.

		ZoneServer* server = player->getZoneServer();

		if (server != NULL) {		

		int index = selectedID == 81 ? 0 : 1;	
		
		// The color index.
		String appearanceFilename = sceneObject->getObjectTemplate()->getAppearanceFilename();
		VectorMap<String, Reference<CustomizationVariable*> > variables;
		AssetCustomizationManagerTemplate::instance()->getCustomizationVariables(appearanceFilename.hashCode(), variables, false);

		// The Sui Box.
		ManagedReference<SuiColorBox*> cbox = new SuiColorBox(player, SuiWindowType::COLOR_ARMOR);
		cbox->setCallback(new ColorArmorSuiCallback(server));
		cbox->setColorPalette(variables.elementAt(index).getKey()); // First one seems to be the frame of it? Skip to 2nd.
		cbox->setUsingObject(sceneObject);

		// Add to player.
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
		ghost->addSuiBox(cbox);
		player->sendMessage(cbox->generateMessage());
		}

	}
	
	return WearableObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
