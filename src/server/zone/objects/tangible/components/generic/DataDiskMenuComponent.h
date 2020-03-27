/*
 * DataDiskMenuComponent
 *
 *  Created on: 24/6/2019
 *      Author: Tyclo
 */

#ifndef DATADISKMENUCOMPONENT_H_
#define DATADISKMENUCOMPONENT_H_

#include "../TangibleObjectMenuComponent.h"
#include "templates/tangible/DataDiskTemplate.h"

class DataDiskMenuComponent : public TangibleObjectMenuComponent {
public:

	/**
	 * Fills the radial options, needs to be overriden
	 * @pre { this object is locked }
	 * @post { this object is locked, menuResponse is complete}
	 * @param menuResponse ObjectMenuResponse that will be sent to the client
	 */
	virtual void fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const;

	/**
	 * Handles the radial selection sent by the client, must be overriden by inherited objects
	 * @pre { this object is locked, player is locked }
	 * @post { this object is locked, player is locked }
	 * @param player PlayerCreature that selected the option
	 * @param selectedID selected menu id
	 * @returns 0 if successfull
	 */
	virtual int handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const;

};


#endif /* DATADISKMENUCOMPONENT_H_ */
