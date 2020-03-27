/*
 * DuplicatorHintMenuComponent
 *
 *  Created on: 02/22/2020
 *      Author: Tyclo
 *  Based on work by: Klivian
 */

#ifndef DUPLICATORHINTMENUCOMPONENT_H_
#define DUPLICATORHINTMENUCOMPONENT_H_

#include "../TangibleObjectMenuComponent.h"

class DuplicatorHintMenuComponent : public TangibleObjectMenuComponent {
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

	String getRarityColor(String rarity) const;

	int getGroupId(bool isSlicing) const;
};


#endif /* DUPLICATORHINTMENUCOMPONENT_H_ */
