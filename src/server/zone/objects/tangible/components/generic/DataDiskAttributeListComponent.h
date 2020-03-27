/*
 * DataDiskAttributeListComponent.h
 *
 *  Created on: 28/6/2019
 *      Author: Tyclo
 */

#ifndef DATADISKATTRIBUTELISTCOMPONENT_H_
#define DATADISKATTRIBUTELISTCOMPONENT_H_

#include "server/zone/objects/scene/components/AttributeListComponent.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/tangible/components/generic/DataDiskDataComponent.h"
#include "templates/tangible/DataDiskTemplate.h"

class DataDiskAttributeListComponent: public AttributeListComponent {
public:

	/**
	 * Fills the Attributes
	 * @pre { this object is locked }
	 * @post { this object is locked, menuResponse is complete}
	 * @param menuResponse ObjectMenuResponse that will be sent to the client
	 */
	void fillAttributeList(AttributeListMessage* alm, CreatureObject* player, SceneObject* sceneObject) const {

		if(!sceneObject->isTangibleObject())
			return;

		ManagedReference<TangibleObject*> tano = cast<TangibleObject*>(sceneObject);

		if (!player->isPlayerCreature())
			return;


		DataObjectComponent* data = tano->getDataObjectComponent()->get();
		if (data == NULL || !data->isDataDiskData())
			return;

		DataDiskDataComponent* dataDisk = cast<DataDiskDataComponent*>(data);
		if (dataDisk == NULL )
			return;

		Reference<DataDiskTemplate*> dataDiskTemplate = cast<DataDiskTemplate*>(sceneObject->getObjectTemplate());
		if (dataDiskTemplate == NULL) {
			return;
		}

		int templateUnlockCode = dataDiskTemplate->getUnlockCode();
		int objectUnlockCode = dataDisk->getUnlockCode();

		if (templateUnlockCode > 0) {
			if (objectUnlockCode != 1) {
				alm->insertAttribute("status", "Locked");
			} else {
				alm->insertAttribute("status", "Unlocked");
			}
		}
	}

};

#endif /* DATADISKATTRIBUTELISTCOMPONENT_H_ */
