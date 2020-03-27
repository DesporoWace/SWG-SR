/*
 * DuplicatorHintAttributeListComponent.h
 *
 *  Created on: 02/22/2020
 *      Author: Tyclo
 */

#ifndef DUPLICATORHINTATTRIBUTELISTCOMPONENT_H_
#define DUPLICATORHINTATTRIBUTELISTCOMPONENT_H_

#include "server/zone/objects/tangible/components/generic/DuplicatorHintDataComponent.h"

class DuplicatorHintAttributeListComponent: public AttributeListComponent {
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
		if (data == NULL)
			return;

		DuplicatorHintDataComponent* dataDisk = cast<DuplicatorHintDataComponent*>(data);
		if (dataDisk == NULL  || !dataDisk->isDuplicatorHintData())
			return;

		int dhID = dataDisk->getId();

		if (dhID >= 0)
			alm->insertAttribute("status", "Unencrypted");
		else
			alm->insertAttribute("status", "Encrypted");
	}

};

#endif /* DUPLICATORHINTATTRIBUTELISTCOMPONENT_H_ */
