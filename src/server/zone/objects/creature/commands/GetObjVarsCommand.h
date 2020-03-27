/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef GETOBJVARSCOMMAND_H_
#define GETOBJVARSCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/chat/ChatManager.h"
#include "server/zone/packets/object/SpatialChat.h"

class GetObjVarsCommand : public QueueCommand {
public:

	GetObjVarsCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		//uint64 objectID = 0;
		uint64 objectID = target;
		UnicodeTokenizer tokenizer(arguments);
		tokenizer.setDelimeter(" ");

		// if we have an argument passed, use it
		if (tokenizer.hasMoreTokens()) {
			String command;
			tokenizer.getStringToken(command);
			command = command.toLowerCase();
			if (command == "snapshot") {
				ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(objectID, false);
				if ( object == NULL) {
					creature->sendSystemMessage("ERROR GETTIGN OBJECT - NULL " + String::valueOf(objectID));
				} else {
					String tempString = object->getObjectTemplate()->getFullTemplateString();
					Quaternion* dir = object->getDirection();

					double roll, pitch, yaw;

					const float Epsilon = 0.0009765625f;
					const float Threshold = 0.5f - Epsilon;

					float XY = dir->getX() * dir->getY();
					float ZW = dir->getZ() * dir->getW();

					float TEST = XY + ZW;

					if (TEST < -Threshold || TEST > Threshold)
					{

						int sign = std::signbit(TEST);

						yaw = sign * 2 * (float)atan2(dir->getX(), dir->getW()) * 180.0/M_PI;

						pitch = sign * M_PI/2.0 * 180.0/M_PI;

						roll = 0;

					}
					else
					{

						float XX = dir->getX() * dir->getX();
						float XZ = dir->getX() * dir->getZ();
						float XW = dir->getX() * dir->getW();

						float YY = dir->getY() * dir->getY();
						float YW = dir->getY() * dir->getW();
						float YZ = dir->getY() * dir->getZ();

						float ZZ = dir->getZ() * dir->getZ();

						roll = (float)atan2(2 * YW - 2 * XZ, 1 - 2 * YY - 2 * ZZ) * 180.0/M_PI;

						pitch = (float)atan2(2 * XW - 2 * YZ, 1 - 2 * XX - 2 * ZZ) * 180.0/M_PI;

						yaw = (float)asin(2 * TEST) * 180.0/M_PI;

					}

					yaw = roundf(yaw);
					pitch = roundf(pitch);
					roll = roundf(roll);

					StringBuffer msg;
					msg << "Snapshot Data for: " << tempString << ", X loc: " << String::valueOf(object->getPositionX()) << ", Y loc: " << String::valueOf(object->getPositionZ()) << ", Z loc: " << String::valueOf(object->getPositionY()) << ", X rotation: " << pitch << ", Y rotation: " << roll << ", Z rotation: " << yaw << endl;
					creature->sendSystemMessage(msg.toString());
				}
			return SUCCESS;
			}
		}
			/*try {
				objectID = tokenizer.getLongToken();
			} catch ( Exception& err ) {
				creature->sendSystemMessage("INVALID OBJECT.  Please specify a valid object name or objectid");
				return INVALIDPARAMETERS;
			}
		} else {
			objectID = target;
		}

		if ( objectID == 0 ) {
			creature->sendSystemMessage("You need to target an object or specify an object id: /getobjvars <objectID> ");
		}*/

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(objectID, false);

		if ( object == NULL) {
			creature->sendSystemMessage("ERROR GETTIGN OBJECT - NULL " + String::valueOf(objectID));
		} else {

			String strClassName = object->getObjectNameStringIdName();
			String strDescription = object->getDetailedDescription();
			String planet = object->getZone()->getZoneName();
			String tempString = object->getObjectTemplate()->getFullTemplateString();
			bool bMarkedForDelete = object->_isMarkedForDeletion();
			bool bIsUpdated = object->_isUpdated();
			int rCount = object.get()->getReferenceCount();
			uint64 parentID = object->getParentID();
			Quaternion* dir = object->getDirection();
			StringBuffer msg;
			StringBuffer smsg;
			smsg << "spawnSceneObject(\"" << planet << "\", \"" << tempString << "\", " << String::valueOf(object->getPositionX()) << ", " << String::valueOf(object->getPositionZ()) << ", " << String::valueOf(object->getPositionY()) << ", " << String::valueOf(parentID) << ", " << dir->getW() << ", " << dir->getX() << ", " << dir->getY() << ", " << dir->getZ() << ")" << endl;

			msg << endl << "OBJECTID: " << String::valueOf(objectID) << endl;
			msg << "OBJECTTYPE: " << String::valueOf(object->getGameObjectType()) << endl;

			if(object->isCreatureObject()) {
				msg << "Creature First Name: " << object.castTo<CreatureObject*>()->getFirstName() << endl;
			}

			msg << "CLASS: " << strClassName << endl;
			msg << "Marked for deletion: " << String::valueOf(bMarkedForDelete) << endl;
			msg << "IsUpdated: " <<  String::valueOf(bIsUpdated) << endl;
			msg << "REFERENCE COUNT " << String::valueOf(rCount) << endl;
			msg << "Path: " << object->getObjectTemplate()->getFullTemplateString() << endl;
			msg << "Children: " << String::valueOf(object->getChildObjects()->size()) << endl;
			msg << "PARENT OBJECTID: " << String::valueOf(parentID) << endl;

			if(object->getZone() != NULL)
				msg << "location: " << String::valueOf(object->getPositionX()) << " "  << String::valueOf(object->getPositionY()) << " " << object->getZone()->getZoneName() << " World Position: " << String::valueOf(object->getWorldPositionX()) << " " << String::valueOf(object->getWorldPositionZ()) << " "  << String::valueOf(object->getWorldPositionY()) << " " << endl;
			ManagedReference<CityRegion*> city = object->getCityRegion().get();
			if (city != NULL)
				msg << "City Region oid: " << String::valueOf(city->getObjectID()) << ", name: " << city->getRegionDisplayedName() << endl;

			creature->sendSystemMessage(smsg.toString());

			ChatManager* chatManager = server->getZoneServer()->getChatManager();
			String title = "getObjVars - " + String::valueOf(objectID);
			chatManager->sendMail("System", title , msg.toString(), creature->getFirstName());
		}


		return SUCCESS;
	}

};

#endif //GETOBJVARSCOMMAND_H_
