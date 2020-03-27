/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef PVPCOMMANDCOMMAND_H_
#define PVPCOMMANDCOMMAND_H_

#include "server/zone/managers/director/DirectorManager.h"

class pvpCommand : public QueueCommand {
public:

	pvpCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		Lua* lua = DirectorManager::instance()->getLuaInstance();

		Reference<LuaFunction*> pvpCommandScreenplay = lua->createFunction("pvpCommandScreenplay", "openInitialWindow", 0);
		*pvpCommandScreenplay << creature;

		pvpCommandScreenplay->callFunction();

		return SUCCESS;
	}

};

#endif //PVPCOMMANDCOMMAND_H_
