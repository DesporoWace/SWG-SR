#ifndef SPLIT_TOKEN_SUI_CALLBACK
#define SPLIT_TOKEN_SUI_CALLBACK

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/SplitTokenSuiCallback.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/objects/tangible/components/generic/VendorTokenMenucomponent.h"
#include "engine/log/Logger.h"

class SplitTokenSuiCallback : public SuiCallback, Logger {
	ManagedWeakReference<SceneObject*> scenObj;

public:
	SplitTokenSuiCallback(ZoneServer* server, SceneObject* sceneObject) : SuiCallback(server) {
		scenObj = sceneObject;
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		if (!player->isPlayerCreature()) {
			return;
		}

		SceneObject* inventory = player->getSlottedObject("inventory");
		if (inventory == NULL) {
			return;
		}

		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isTransferBox() || cancelPressed || args->size() <= 1) {
			return;
		}

		int newCount = Integer::valueOf(args->get(1).toString());
		SceneObject* sceno = scenObj.get();

		String fullTemplate = sceno->getObjectTemplate()->getFullTemplateString();
		TangibleObject* tano = cast <TangibleObject* >(sceno);
		ManagedReference<TangibleObject*> newToken = player->getZoneServer()->createObject(fullTemplate.hashCode(), 1).castTo<TangibleObject*>();

		if (newToken == NULL) {
			return;
		}

		Locker locker(tano);

		int tokenCount = tano->getUseCount();

		if (tokenCount <= 0 or newCount <= 0 or ((tokenCount - newCount) <= 0)) {
			player->sendSystemMessage("Please enter a stack size greater than 0.");
			setLoggingName("SplitToken");
			warning(player->getFirstName() + " (Attempted to Split a Token by 0)");
			return;
		}

		tano->setUseCount(tokenCount - newCount, true);

		Locker newTokenLocker(newToken);

		inventory->transferObject(newToken, -1, true);

		newToken->sendTo(player, true);
		newToken->setUseCount(newCount, true);

		player->sendSystemMessage("You've successfully split your items.");
	}
};

#endif
