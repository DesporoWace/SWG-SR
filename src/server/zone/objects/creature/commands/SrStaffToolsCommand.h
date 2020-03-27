/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SRSTAFFTOOLSCOMMAND_H_
#define SRSTAFFTOOLSCOMMAND_H_

#include "server/zone/managers/director/DirectorManager.h"

class SrStaffToolsCommand : public QueueCommand {
public:

	SrStaffToolsCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		Lua* lua = DirectorManager::instance()->getLuaInstance();

		Reference<LuaFunction*> srStaffToolsScreenplay = lua->createFunction("srStaffToolsScreenplay", "openInitialWindow", 0);
		*srStaffToolsScreenplay << creature;

		srStaffToolsScreenplay->callFunction();

		return SUCCESS;
	}

};

#endif //SRSTAFFTOOLSCOMMAND_H_
