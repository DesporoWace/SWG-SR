/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SURRENDERSKILLCOMMAND_H_
#define SURRENDERSKILLCOMMAND_H_

#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/managers/director/DirectorManager.h"

class SurrenderSkillCommand : public QueueCommand {
public:

	SurrenderSkillCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;
			
		if (arguments.toString() == "combat_bountyhunter_master" || arguments.toString() == "combat_commando_master" || arguments.toString() == "outdoors_squadleader_master" || arguments.toString() == "outdoors_ranger_master"){
			Lua* lua = DirectorManager::instance()->getLuaInstance();
			LuaFunction runMethod(lua->getLuaState(), "srCustomChecks", "checkMandalorianArmor", 1);
			runMethod << creature;

			runMethod.callFunction();

			int result = lua_tointeger(lua->getLuaState(), -1);

			lua_pop(lua->getLuaState(), 1);

			if (result == 0)
				return INVALIDSTATE;
		}

		SkillManager* skillManager = SkillManager::instance();
		skillManager->surrenderSkill(arguments.toString(), creature, true);

		return SUCCESS;
	}

};

#endif //SURRENDERSKILLCOMMAND_H_

