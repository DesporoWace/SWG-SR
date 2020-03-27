
#include "HoloCostumeMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/managers/director/DirectorManager.h"


void HoloCostumeMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	menuResponse->addRadialMenuItem(20, 3, "Costume Menu");

}

int HoloCostumeMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(player)) {
		player->sendSystemMessage("The holographic costume can't be applied unless in your inventory.");
		return 0;
	}

	if (selectedID == 20) {
		Lua* lua = DirectorManager::instance()->getLuaInstance();
		
		Reference<LuaFunction*> srHoloCostumeScreenplay = lua->createFunction("srHoloCostumeScreenplay", "openInitialWindow", 0);
		*srHoloCostumeScreenplay << player;

		srHoloCostumeScreenplay->callFunction();
	}
	
	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
