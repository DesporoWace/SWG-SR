/*
 * DestroyStructureRequestSuiCallback.h
 *
 *  Created on: Jun 22, 2011
 *      Author: crush
 */

#ifndef DESTROYSTRUCTUREREQUESTSUICALLBACK_H_
#define DESTROYSTRUCTUREREQUESTSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sessions/DestroyStructureSession.h"

class DestroyStructureRequestSuiCallback : public SuiCallback {
private:
    bool request_code = true;

public:
	DestroyStructureRequestSuiCallback(ZoneServer* serv, bool request_code=true) : SuiCallback(serv) {
	    this->request_code = request_code;
	}

	void run(CreatureObject* player, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		ManagedReference<DestroyStructureSession*> session = player->getActiveSession(SessionFacadeType::DESTROYSTRUCTURE).castTo<DestroyStructureSession*>();

		if (session == NULL)
			return;

		if (cancelPressed) {
			session->cancelSession();
			return;
		}

		if(this->request_code)
            session->sendDestroyCode();
		else
            session->destroyStructure();
	}
};

#endif /* DESTROYSTRUCTUREREQUESTSUICALLBACK_H_ */
