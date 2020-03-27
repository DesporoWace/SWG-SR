#ifndef HARVESTERRESOURCEEXTRACTORHOPPERSUICALLBACK_H_
#define HARVESTERRESOURCEEXTRACTORHOPPERSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/sui/callbacks/HarvesterResourceExtractorRetrievalSuiCallback.h"
#include "server/zone/objects/installation/harvester/HarvesterObject.h"
#include "server/zone/objects/installation/InstallationObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/objects/resource/ResourceSpawn.h"

class HarvesterResourceExtractorHopperSuiCallback : public SuiCallback {
	ManagedReference<SceneObject*> sceneObject;

public:
	HarvesterResourceExtractorHopperSuiCallback(ZoneServer* server, SceneObject* sceneObject)
		: SuiCallback(server) {

		this->sceneObject = sceneObject;
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (!suiBox->isListBox() || cancelPressed)
			return;

		if (args->size() < 1)
			return;

		int index = Integer::valueOf(args->get(0).toString());

		if (index == -1)
			return;

		if (!player->isPlayerCreature())
			return;

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		if (ghost->hasSuiBoxWindowType(SuiWindowType::HARVESTER_COMM))
			return;

		if (sceneObject == NULL)
			return;

		if (!sceneObject->isTangibleObject())
			return;

		SuiListBox* listBox = cast<SuiListBox*>(suiBox);
		uint64 harvesterObjectID = listBox->getMenuObjectID(index);

		ManagedReference<StructureObject*> structure = server->getObject(harvesterObjectID).castTo<StructureObject*>();

		if (structure == NULL)
			return;

		if (!structure->isInstallationObject())
			return;

		String templateFile = structure->getObjectTemplate()->getFullTemplateString();
		StringIdManager* sidman = StringIdManager::instance();
		String harvType = sidman->getStringId("@installation_n:" + structure->getObjectNameStringIdName()).toString();;
		String harvName = structure->getCustomObjectName().toString();

		if (harvName == "" && harvType != "")
			harvName = harvType;

		String operational = "\\#ff4444(Offline)";

		ManagedReference<InstallationObject*> installationObj = structure.castTo<InstallationObject*>();
		if (installationObj->isOperating())
			operational = "\\#47ef77(Online)";

		Zone* zone = structure->getZone();

		if (zone == NULL)
			return;

		ManagedReference<SuiListBox*> suiHopperBox = new SuiListBox(player, SuiWindowType::HARVESTER_COMM, SuiListBox::HANDLETHREEBUTTON);
		suiHopperBox->setPromptTitle("Hopper: " + harvName + " - " + harvType);
		suiHopperBox->setUsingObject(structure);
		suiHopperBox->setForceCloseDisabled();

		if (installationObj == NULL)
			return;

		Locker locker(installationObj);
		int resourceCount = 0;
		try {
			if (installationObj->getZone() == NULL)
				return;

			// Fetch latest resources
			installationObj->updateInstallationWork();

			HopperList* list = installationObj->getHopperList();

			for (int i = 0; i < list->size(); ++i) {
				ResourceSpawn* ri = list->get(i)->getSpawnObject();
				ManagedReference<ResourceContainer*> container = installationObj->getContainerFromHopper(ri);

				suiHopperBox->addMenuItem("\\#ffe254" + ri->getFamilyName() + " \\#cee5e5(\\#ffffff" + ri->getFinalClass() + "\\#cee5e5) \\#ffffff" + ri->getName() + " \\#cee5e5| \\#ffffff" + container->getQuantity() + " \\#fff1bcunits", ri->getObjectID());
				resourceCount++;
			}
		} catch (Exception& e) {
			System::out << e.getMessage();
			e.printStackTrace();
		}

		String promptText = "\\#ffe254" + harvName + " \\#ffff3f\n\\#cee5e5Type:\t\t\t\\#ffffff" + harvType + "\n\\#cee5e5Operational:\t" + operational;
		if (resourceCount != 0) {
			promptText += "\n\n\\#ffffffSelect a resource from the list below and choose \\#fff1bc\"Retrieve Resource\"\\#ffffff to collect the selected resource from the harvester's hopper into your inventory.";
			promptText += "\n\nRetrieving a resource will use a charge on this device. You many only retrieve up to \\#fff1bc" + String::valueOf(ResourceContainer::MAXSIZE) + " \\#ffffffunits at a time.";
			suiHopperBox->setOkButton(true, "@ui:res_harvest_retrieve"); // Retrieve Resource
			suiHopperBox->setOtherButton(true, "@ui:back");
			suiHopperBox->setCancelButton(true, "@cancel");
			suiHopperBox->setPromptText(promptText);
			suiHopperBox->setCallback(new HarvesterResourceExtractorRetrievalSuiCallback(player->getZoneServer(), sceneObject));
			ghost->addSuiBox(suiHopperBox);
			player->sendMessage(suiHopperBox->generateMessage());
		} else {
			// No resources
			player->sendSystemMessage("No resources were found in the hopper of the harvester \\#ffe254" + harvName + "\\#ffffff. Returning to main menu.");
			ManagedReference<TangibleObject*> tano = sceneObject.castTo<TangibleObject*>();
			tano->handleObjectMenuSelect(player, 22); // Do not decrement charge
		}
	}

	String getTimeString(uint32 timestamp) const {

		if( timestamp == 0 ){
			return "";
		}

		static const String abbrvs[3] = { "minutes", "hours", "days" };

		static const int intervals[3] = { 60, 3600, 86400 };
		int values[3] = { 0, 0, 0 };

		StringBuffer str;

		for (int i = 2; i > -1; --i) {
			values[i] = floor((float) timestamp / intervals[i]);
			timestamp -= values[i] * intervals[i];

			if (values[i] > 0) {
				if (str.length() > 0){
					str << ", ";
				}

				str << values[i] << " " << abbrvs[i];
			}
		}

		return "(" + str.toString() + ")";
	}
};

#endif /* HARVESTERRESOURCEEXTRACTORHOPPERSUICALLBACK_H_ */
