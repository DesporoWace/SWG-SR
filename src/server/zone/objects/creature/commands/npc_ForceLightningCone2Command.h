/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef npc_FORCELIGHTNINGCONE2COMMAND_H_
#define npc_FORCELIGHTNINGCONE2COMMAND_H_

#include "ForcePowersQueueCommand.h"

class npc_ForceLightningCone2Command : public ForcePowersQueueCommand {
public:

	npc_ForceLightningCone2Command(const String& name, ZoneProcessServer* server)
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

		return doCombatAction(creature, target);
	}

};

#endif //npc_FORCELIGHTNINGCONE2COMMAND_H_
