
#include "HoloCostumeQuestMenuComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/managers/director/DirectorManager.h"


void HoloCostumeQuestMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

	menuResponse->addRadialMenuItem(20, 3, "Seek Lesson");
	menuResponse->addRadialMenuItem(21, 3, "Seek Guidance");

}

int HoloCostumeQuestMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!player->isPlayerCreature()) {
		return 0;
	}

	if (!sceneObject->isASubChildOf(player)) {
		player->sendSystemMessage("The Holocron of Destiny cannot be used out of your inventory.");
		return 0;
	}

	if (selectedID == 20) {
		Lua* lua = DirectorManager::instance()->getLuaInstance();

		Reference<LuaFunction*> srHoloCostumeAnniversaryQuestScreenplay = lua->createFunction("srHoloCostumeAnniversaryQuestScreenplay", "checkPlanet", 0);
		*srHoloCostumeAnniversaryQuestScreenplay << player;

		srHoloCostumeAnniversaryQuestScreenplay->callFunction();
	}
	if (selectedID == 21) {
		Lua* lua = DirectorManager::instance()->getLuaInstance();

		Reference<LuaFunction*> srHoloCostumeAnniversaryQuestScreenplay = lua->createFunction("srHoloCostumeAnniversaryQuestScreenplay", "getNextPlanet", 0);
		*srHoloCostumeAnniversaryQuestScreenplay << player;

		srHoloCostumeAnniversaryQuestScreenplay->callFunction();
	}
	
	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
