
#include "HoloCostumeChargeMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/managers/director/DirectorManager.h"


void HoloCostumeChargeMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	menuResponse->addRadialMenuItem(20, 3, "Add Holo Charges");

}

int HoloCostumeChargeMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(player)) {
		player->sendSystemMessage("You can't add holo charges if the item is not in your inventory.");
		return 0;
	}

	if (selectedID == 20) {
		Lua* lua = DirectorManager::instance()->getLuaInstance();
		
		Reference<LuaFunction*> srHoloCostumeScreenplay = lua->createFunction("srHoloCostumeScreenplay", "addHoloCharge", 0);
		*srHoloCostumeScreenplay << player;
		*srHoloCostumeScreenplay << sceneObject;

		srHoloCostumeScreenplay->callFunction();
	}
	
	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
