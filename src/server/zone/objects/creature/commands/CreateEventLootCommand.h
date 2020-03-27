/*
		Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CreateEventLootCOMMAND_H_
#define CreateEventLootCOMMAND_H_

#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/objects/scene/SceneObject.h"

class CreateEventLootCommand : public QueueCommand {
public:

	CreateEventLootCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		StringTokenizer args(arguments.toString());
		try {
			String lootGroup = "g_event"; // fallback
			int groupChance = System::random(10);

			String lootGroupArray[3] = {
				"event_paintings",
				"event_attachments_all",
				"event_jedi",
			};
			int lootGroupWeights[3] = {
				5,
				3,
				2,
			};
			int lootGroupWeightSum = 0;
			// Set length
			for (int i = 0; i < 3; i++) {
				lootGroupWeightSum += lootGroupWeights[i];
				if (groupChance <= lootGroupWeightSum) {
					lootGroup = lootGroupArray[i];
					break;
				}
			}

			int range = 64;
			if (args.hasMoreTokens())
				range = args.getIntToken();

			if( range < 0 )
				range = 32;

			if( range > 128 )
				range = 128;

			int level = 0;
			if (args.hasMoreTokens())
				level = args.getIntToken();

			if (level > 250)
				level = 250;

			if (level == 0) {
				if (System::random(4) == 1)
					level = 250; // 33%
				else
					level = 150; // 66%
			}

			ManagedReference<LootManager*> lootManager = creature->getZoneServer()->getLootManager();
			if (lootManager == NULL)
				return INVALIDPARAMETERS;

			Zone* zone = creature->getZone();
			if (zone == NULL)
				return GENERALERROR;

			// Find all objects in range
			SortedVector<QuadTreeEntry*> closeObjects;
			CloseObjectsVector* closeObjectsVector = (CloseObjectsVector*) creature->getCloseObjects();
			if (closeObjectsVector == NULL) {
				zone->getInRangeObjects(creature->getPositionX(), creature->getPositionY(), range, &closeObjects, true);
			} else {
				closeObjectsVector->safeCopyTo(closeObjects);
			}

			// Award loot group to all players in range
			int closePlayersCount = 0;
			for (int i = 0; i < closeObjects.size(); i++) {
				SceneObject* targetObject = static_cast<SceneObject*>(closeObjects.get(i));

				if (targetObject->isPlayerCreature() && creature->isInRange(targetObject, range)) {

					closePlayersCount++;
					CreatureObject* targetPlayer = cast<CreatureObject*>(targetObject);
					Locker tlock( targetPlayer, creature );

					ManagedReference<SceneObject*> inventory = targetPlayer->getSlottedObject("inventory");
					if (inventory != NULL) {
						if( lootManager->createLoot(inventory, lootGroup, level) )
							targetPlayer->sendSystemMessage( " \\#f6d53b[Event] \\#ffffffYou have received a loot item!" );
					}

					tlock.release();
				}
			}
			// Send system message to admin
			creature->sendSystemMessage( " \\#ff0000[Admin Event Loot] \\#ffffffDistributed loot group \\#ffd27f" + lootGroup + " \\#fffffflevel \\#ffe254" + level + " \\#ffffffto \\#ffd27f" + closePlayersCount + " \\#ffffffplayers." );
		} catch (Exception& e) {
			creature->sendSystemMessage("SYNTAX: /createEventLoot [<range>] [<level>]");

			return INVALIDPARAMETERS;
		}
		return SUCCESS;
	}

};

#endif //CreateEventLootCOMMAND_H_
