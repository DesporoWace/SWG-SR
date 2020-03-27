/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SrChatCommand_H_
#define SrChatCommand_H_

#include "server/chat/ChatManager.h"

class SrChatCommand : public QueueCommand {
public:

	SrChatCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ChatManager* chatManager = server->getZoneServer()->getChatManager();
		chatManager->handleSRChat( creature, arguments);

		return SUCCESS;
	}

};

#endif //SrChatCommand_H_
