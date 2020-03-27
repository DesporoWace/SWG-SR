#ifndef PLAYER_BH_SUI_CALLBACK
#define PLAYER_BH_SUI_CALLBACK

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/managers/visibility/VisibilityManager.h"
#include "server/zone/objects/player/sui/callbacks/BountyHuntSuiCallback.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/managers/mission/MissionManager.h"

class BountyHuntSuiCallback : public SuiCallback {

public:
	BountyHuntSuiCallback(ZoneServer* serv) : SuiCallback(serv) {}

void run(CreatureObject* creature, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);
		int bountyMax = 1500000;
		int newBounty = 0;
		int refund = 0;
		int currentBounty = 0;

		if (creature == NULL || cancelPressed) {
			return;
		}

		try {
			int value = Integer::valueOf(args->get(0).toString());

			if (value <= 25000)
				value = 25000;

			ManagedReference<SceneObject*> suiObject = suiBox->getUsingObject();
			CreatureObject* player = cast<CreatureObject*>(suiObject.get());
			uint64 id = player->getObjectID();

			PlayerManager* playerManager = creature->getZoneServer()->getPlayerManager();
			MissionManager* missionManager = creature->getZoneServer()->getMissionManager();

			if (missionManager->hasPlayerBountyTargetInList(id)) {
				currentBounty = missionManager->getPlayerBounty(player->getObjectID());
			}

			newBounty = currentBounty + (value * .8); // Fee for BH

			if (creature->getBankCredits() + creature->getCashCredits() >= value) {
				if (creature->getBankCredits() > value) {
					creature->subtractBankCredits(value);
				} else {
					creature->subtractCashCredits(value - creature->getBankCredits());
					creature->subtractBankCredits(creature->getBankCredits());
				}

				if (newBounty >= bountyMax) {
					refund = newBounty - bountyMax;
					newBounty = bountyMax;
				}

				VisibilityManager::instance()->clearVisibility(player);
				VisibilityManager::instance()->increaseVisibility(player, 8000);

				if (!missionManager->hasPlayerBountyTargetInList(id)) {
					missionManager->addPlayerToBountyList(id, newBounty);
					missionManager->updatePlayerBountyOnlineStatus(id, true);
				} else {
					missionManager->updatePlayerBountyReward(id, newBounty);
				}

				creature->sendSystemMessage(" \\#f0f497[Bounty Set]\\#ffffff You have successfully placed a \\#ffd27f" + String::valueOf(value) + "\\#ffffff credit bounty on \\#ffd27f" + player->getFirstName() + " \\#ffffff.");
				player->playEffect("clienteffect/ui_missile_aquiring.cef", "head");

				if (refund > 0) {
					creature->sendSystemMessage(" \\#ffa500[NOTICE] \\#ffffffYour bounty and others placed exceeds the maximum number of credits the Bounty Hunters Guild allows, you have been refunded \\#ffd27f" + String::valueOf(refund) + " \\#ffffffcredits to your bank.");
					creature->addBankCredits(refund);
				}
			}
			else
				creature->sendSystemMessage(" \\#ff0000[NOTICE] \\#ffffffYou have insufficient funds. You need at least 25,000 credits in Cash or Bank. No bounty was placed.");
		} catch(Exception& e) { }
	}
};

#endif
