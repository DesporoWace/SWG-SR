/*
 * InstallationBeaconMenuComponent
 *
 *  Created on: 24/03/2019
 *      Author: Tyclo
 */

#include "InstallationBeaconMenuComponent.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/installation/harvester/HarvesterObject.h"
#include "server/zone/objects/installation/factory/FactoryObject.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/managers/stringid/StringIdManager.h"

void InstallationBeaconMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);

}

int InstallationBeaconMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {
	if (!sceneObject->isTangibleObject())
		return 0;

	if (!player->isPlayerCreature())
		return 0;

	if (selectedID == 20) {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (!sceneObject->isASubChildOf(inventory)) {
			player->sendSystemMessage("In order to operate the UniTech Installation Beacon the item must be in your inventory");
			return 0;
		}

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return 0;

		if (ghost->hasSuiBoxWindowType(SuiWindowType::INSTALLATION_BEACON))
			return 0;

		StringBuffer body;
		StringBuffer harvesters;
		StringBuffer generators;
		StringBuffer factories;
		int count = 0;
		int harvCount = 0;
		int genCount = 0;
		int factoryCount = 0;
		int tempCount = 0;

		StringIdManager* sidman = StringIdManager::instance();

		for (int i = 0; i < ghost->getTotalOwnedStructureCount(); i++) {
			ManagedReference<StructureObject*> structure = player->getZoneServer()->getObject(ghost->getOwnedStructure(i)).castTo<StructureObject*>();

			if (structure == NULL || !structure->isInstallationObject() || structure->isCivicStructure())
				continue;

			StringBuffer temp;

			count++;

			String installType = sidman->getStringId("@" + structure->getObjectNameStringIdFile() + ":" + structure->getObjectNameStringIdName()).toString();
			String installName = structure->getCustomObjectName().toString();

			if (installName == "" && installType != "")
				installName = installType;

			if (structure->isHarvesterObject()) {
				harvCount++;
				tempCount = harvCount;
			} else if (structure->isGeneratorObject()) {
				genCount++;
				tempCount = genCount;
			} else if (structure->isFactory()) {
				factoryCount++;
				tempCount = factoryCount;
			} else
				continue;

			temp << "\\#ffffff" << tempCount << ") \\#ffe254" << installName << endl;
			temp << "\t\\#cee5e5Type:\t\t\t\t\\#ffffff" << installType << endl;

			String operating = "\\#ff4444Offline";

			ManagedReference<InstallationObject*> installationObj = structure.castTo<InstallationObject*>();
			if (installationObj->isOperating())
				operating = "\\#47ef77Online";

			temp << "\t\\#cee5e5Operational:\t\t" << operating << endl;

			Zone* zone = structure->getZone();

			if (zone == NULL)
				return 0;

			float propertytax = 0.f; // property tax
			ManagedReference<CityRegion*> city = structure->getCityRegion().get();
			if (city != NULL)
				propertytax = city->getPropertyTax() / 100.f * structure->getMaintenanceRate();

			float secsRemainingMaint = 0.f; // maintenance
			if (structure->getSurplusMaintenance() > 0 ){
				float totalrate = (float)structure->getMaintenanceRate() + propertytax;
				secsRemainingMaint = ((float)structure->getSurplusMaintenance() / totalrate)*3600;
			}

			temp << "\t\\#cee5e5Maintainence:\t\t\\#ffffff" << String::valueOf( (int) floor( (float) structure->getSurplusMaintenance())) << " \\#fff1bccredits \\#ffffff" << getTimeString( (uint32)secsRemainingMaint ) << endl;

			if (!structure->isGeneratorObject()) {
				float secsRemainingPower = 0.f;
				float basePowerRate = structure->getBasePowerRate();
				if ((structure->getSurplusPower() > 0) && (basePowerRate != 0))
					secsRemainingPower = ((float)structure->getSurplusPower() / (float)basePowerRate)*3600;

				temp << "\t\\#cee5e5Power:\t\t\t\t\\#ffffff" << String::valueOf( (int) structure->getSurplusPower()) << " \\#fff1bcunits \\#ffffff" << getTimeString( (uint32)secsRemainingPower ) << endl;
			}

			if (zone == NULL) {
				temp << "\t\\#cee5e5Zone:\t\t\t\t\\#ffffffUnknown" << endl;
			} else {
				temp << "\t\\#cee5e5Zone:\t\t\t\t\\#ffffff" << zone->getZoneName() << endl;
				temp << "\t\\#cee5e5World Position:\\#ffffff\t\t" << String::valueOf(structure->getWorldPositionX()) + "\\#fff1bc, \\#ffffff" << String::valueOf(structure->getWorldPositionY()) << endl;
			}

			if (structure->isHarvesterObject() || structure->isGeneratorObject()) {
				ManagedReference<InstallationObject*> hino = structure.castTo<InstallationObject*>();

				if (hino == NULL)
					return 0;

				Locker locker(hino);

				try {
					if (hino->getZone() == NULL)
						return 0;

					// Fetch latest resources
					hino->updateInstallationWork();

					long resourceId = hino->getActiveResourceSpawnID();

					if (resourceId != 0) {
						ManagedReference<ResourceSpawn*> resourceSpawn = hino->getZoneServer()->getObject(resourceId).castTo<ResourceSpawn*>();

						temp << "\t\\#cee5e5Active Resource:\\#ffffff\t" << resourceSpawn->getFamilyName() << endl;
						temp << "\t\\#cee5e5Resource Type:\\#ffffff\t\t" << resourceSpawn->getFinalClass() << endl;
						temp << "\t\\#cee5e5Resource Name:\\#ffffff\t\t" << resourceSpawn->getName() << endl;
					} else {
						temp << "\t\\#cee5e5Active Resource:\\#ffffff\tNone" << endl;
					}

					temp << "\t\\#cee5e5Hopper:\\#ffffff\t\t\t\t" << String::valueOf(hino->getHopperSize()) << " \\#fff1bcout of \\#ffffff" << String::valueOf(hino->getHopperSizeMax()) << endl;
					temp << "\t\\#cee5e5Extraction Rate:\\#ffffff\t\t" << String::valueOf(hino->getActualRate()) << " \\#fff1bcout of \\#ffffff" << String::valueOf(Math::getPrecision(hino->getExtractionRate(), 2)) << endl;
				} catch (Exception& e) {
					System::out << e.getMessage();
					e.printStackTrace();
				}
			}

			if (structure->isFactory()) {
				ManagedReference<FactoryObject*> factoryObj = structure.castTo<FactoryObject*>();
				Reference<SceneObject*> hopper = factoryObj->getSlottedObject("ingredient_hopper");

				if (hopper == NULL)
					return 0;

				String hopperItems = "\\#ff4444Hopper Full";

				int hopperCount = hopper->getCountableObjectsRecursive();

				if (hopper->getContainerVolumeLimit() > hopperCount)
					hopperItems = String::valueOf(hopperCount) + " / " + String::valueOf(hopper->getContainerVolumeLimit());

				temp << "\t\\#cee5e5Hopper Size:\\#ffffff\t\t\t" << hopperItems << endl;

				if (factoryObj->getContainerObjectsSize() != 0) {
					ManagedReference<ManufactureSchematic* > schematic = factoryObj->getContainerObject(0).castTo<ManufactureSchematic*>();

					String schematicName = "";
					String schematicType = sidman->getStringId("@" + schematic->getObjectNameStringIdFile() + ":" + schematic->getObjectNameStringIdName()).toString();
					if (schematic->getCustomObjectName().isEmpty())
						schematicName = schematicType;
					else
						schematicName = schematic->getCustomObjectName().toString();

					temp << "\t\\#cee5e5Active Schematic:\\#ffffff\t" << schematicName << endl;
					temp << "\t\\#cee5e5Schematic Type:\\#ffffff\t\t" << schematicType << endl;
					temp << "\t\\#cee5e5Schematic Limit:\\#ffffff\t" << schematic->getManufactureLimit() << endl;
				} else {
					temp << "\t\\#cee5e5Schematic:\\#ffffff\t\t\tNone" << endl;
				}
			}

			if (structure->isHarvesterObject())
				harvesters << temp.toString();
			else if (structure->isGeneratorObject())
				generators << temp.toString();
			else if (structure->isFactory())
				factories << temp.toString();

		} // end loop

		if (count == 0) {
			player->sendSystemMessage("You do not own any installations that are compatible with this device.");
			return 0;
		}

		body << "\\#cee5e5Total Installations: \\#ffffff" << String::valueOf(count) << endl << endl;

		if (harvesters.toString() != "")
			body << "\\#fff1bcHarvesters: \\#ffffff" << String::valueOf(harvCount) << endl << harvesters.toString() << endl << endl;
		if (generators.toString() != "")
			body << "\\#fff1bcGenerators: \\#ffffff" << String::valueOf(genCount) << endl << generators.toString() << endl << endl;
		if (factories.toString() != "")
			body << "\\#fff1bcFactories: \\#ffffff" << String::valueOf(factoryCount) << endl << factories.toString() << endl << endl;

		ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::INSTALLATION_BEACON);
		box->setPromptTitle(player->getFirstName() + "'s Installations");
		box->setPromptText(body.toString());
		box->setOkButton(true, "@close");
		box->setUsingObject(player);
		box->setForceCloseDisabled();

		ghost->addSuiBox(box);
		player->sendMessage(box->generateMessage());

		TangibleObject* tano = cast<TangibleObject*>(sceneObject);

		Locker locker(tano);
		int useCount = tano->getUseCount();
		tano->decreaseUseCount();
		useCount--;

		if (useCount == 0)
			player->sendSystemMessage("The UniTech Installation Beacon has used its last charge and has been destroyed.");

		return 0;
	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}

String InstallationBeaconMenuComponent::getTimeString(uint32 timestamp) const {

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
