/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SREVENTMENUCOMMAND_H_
#define SREVENTMENUCOMMAND_H_

#include "server/zone/managers/director/DirectorManager.h"

class SrEventMenuCommand : public QueueCommand {
public:

	SrEventMenuCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		Lua* lua = DirectorManager::instance()->getLuaInstance();

		Reference<LuaFunction*> srEventMenuScreenplay = lua->createFunction("srEventMenuScreenplay", "openInitialWindow", 0);
		*srEventMenuScreenplay << creature;

		srEventMenuScreenplay->callFunction();

		return SUCCESS;
	}

};

#endif //SREVENTMENUCOMMAND_H_
