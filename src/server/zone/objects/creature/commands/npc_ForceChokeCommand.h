/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef npc_FORCECHOKECOMMAND_H_
#define npc_FORCECHOKECOMMAND_H_

#include "ForcePowersQueueCommand.h"

class npc_ForceChokeCommand : public ForcePowersQueueCommand {
public:

	npc_ForceChokeCommand(const String& name, ZoneProcessServer* server)
		: ForcePowersQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);

		if (targetObject == NULL || !targetObject->isCreatureObject()) {
			return INVALIDTARGET;
		}

		return doCombatAction(creature, target);

	}

};

#endif //npc_FORCECHOKECOMMAND_H_
