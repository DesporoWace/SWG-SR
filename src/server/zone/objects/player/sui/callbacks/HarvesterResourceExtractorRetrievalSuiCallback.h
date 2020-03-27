#ifndef HARVESTERRESOURCEEXTRACTORRETRIEVALSUICALLBACK_H_
#define HARVESTERRESOURCEEXTRACTORRETRIEVALSUICALLBACK_H_

#include "server/zone/packets/BaseLineMessage.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/Zone.h"
#include "server/zone/packets/object/ObjectControllerMessageCallback.h"
#include "server/zone/objects/installation/harvester/HarvesterObject.h"
#include "server/zone/packets/harvester/HarvesterObjectMessage7.h"

class HarvesterResourceExtractorRetrievalSuiCallback : public SuiCallback {
	uint64 harvesterId;
	uint64 resourceId;
	uint32 quantity;
	uint8 byte1;
	uint8 byte2;

	ObjectControllerMessageCallback* objectControllerMain;
	ManagedReference<SceneObject*> sceneObject;

public:
	HarvesterResourceExtractorRetrievalSuiCallback(ZoneServer* server, SceneObject* sceneObject)
		: SuiCallback(server) {

		this->sceneObject = sceneObject;
	}

	void run(CreatureObject* creature, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {

		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isListBox() || cancelPressed)
			return;

		if (args->size() < 2)
			return;

		bool otherPressed = Bool::valueOf(args->get(0).toString());
		int index = Integer::valueOf(args->get(1).toString());

		if (creature == NULL)
			return;

		if (!creature->isPlayerCreature())
			return;

		CreatureObject* player = cast<CreatureObject*>(creature);

		if (otherPressed) {
			// Back
			if (sceneObject == NULL)
				return;

			if (!sceneObject->isTangibleObject())
				return;

			ManagedReference<TangibleObject*> tano = sceneObject.castTo<TangibleObject*>();
			tano->handleObjectMenuSelect(player, 22); // Do not decrement charge
		} else {
			if (index != -1) {
				SuiListBox* listBox = cast<SuiListBox*>(suiBox);
				uint64 resourceId = listBox->getMenuObjectID(index);

				ManagedReference<SceneObject*> object = listBox->getUsingObject().get();

				if (object == NULL)
					return;

				InstallationObject* inso = cast<InstallationObject*>(object.get());

				if (inso == NULL)
					return;

				if (!inso->isInstallationObject())
					return;

				try {
					Locker clocker(inso, player);

					if (!inso->isOnAdminList(player))
						return;

					SceneObject* inventory = player->getSlottedObject("inventory");

					ManagedReference<ResourceSpawn*> resourceSpawn = object->getZoneServer()->getObject(resourceId).castTo<ResourceSpawn*>();

					if (resourceSpawn == NULL) {
						player->error("wrong spawn id - Please report this");
						return;
					}

					ManagedReference<ResourceContainer*> container = inso->getContainerFromHopper(resourceSpawn);

					if (container == NULL) {
						player->error("null container - Please report this");
						return;
					}

					if (container->getQuantity() == 0) {
						player->sendSystemMessage("Unable to retrieve a resource with a quantity of 0");
						return;
					}

					int quantity = container->getQuantity();

					if (quantity > ResourceContainer::MAXSIZE)
						quantity = ResourceContainer::MAXSIZE;

					if (!inventory->isContainerFullRecursive()) {
						Reference<ResourceSpawn*> resSpawn = container->getSpawnObject();
						Locker locker(resSpawn);

						ManagedReference<ResourceContainer*> newContainer = resSpawn->createResource(quantity);
						if (inventory->transferObject(newContainer, -1, false)) {
							inventory->broadcastObject(newContainer, true);

							inso->updateResourceContainerQuantity(container, container->getQuantity() - quantity, true);
						} else {
							newContainer->destroyObjectFromDatabase(true);
						}
					} else {
						StringIdChatParameter stringId("error_message", "inv_full");
						player->sendSystemMessage(stringId);
						return;
					}

					inso->broadcastToOperators(new HarvesterObjectMessage7(inso));

					player->sendSystemMessage("You have successfully retrieved " + String::valueOf(quantity) + " units of " + resourceSpawn->getName() + " " + resourceSpawn->getFamilyName());
				} catch (Exception& e) {
					player->error("unreported exception caught in HarvesterResourceExtractorRetrievalSuiCallback::run - Please report this");
					System::out << e.getMessage();
					e.printStackTrace();
				}
			}
		}

		return;

	}
};

#endif /* HARVESTERRESOURCEEXTRACTORRETRIEVALSUICALLBACK_H_ */
