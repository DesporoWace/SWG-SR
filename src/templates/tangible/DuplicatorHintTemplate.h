/*
 * DuplicatorHintTemplate.h
 *
 *  Created on: 02/22/2020
 *      Author: Tyclo
 */

#ifndef DUPLICATORHINTTEMPLATE_H_
#define DUPLICATORHINTTEMPLATE_H_

#include "templates/SharedTangibleObjectTemplate.h"

class DuplicatorHintTemplate: public SharedTangibleObjectTemplate {
protected:

	int id;

public:
	DuplicatorHintTemplate() {
		id = -1;
	}

	~DuplicatorHintTemplate() {

	}

	void readObject(LuaObject* templateData) {
		SharedTangibleObjectTemplate::readObject(templateData);
	}

	int getId() {
		return id;
	}

	bool isDuplicatorHintTemplate() {
		return true;
	}

};

#endif /* DUPLICATORHINTTEMPLATE_H_ */
