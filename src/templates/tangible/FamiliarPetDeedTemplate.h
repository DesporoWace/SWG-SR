/*
 * FamiliarPetDeedTemplate.h
 *
 *  Created on: August 17, 2013
 *      Author: washu
 */

#ifndef FamiliarPetDeedTemplate_H_
#define FamiliarPetDeedTemplate_H_

#include "templates/tangible/DeedTemplate.h"

class FamiliarPetDeedTemplate : public DeedTemplate {
private:
	String controlDeviceObjectTemplate;
	String mobileTemplate;

public:
	FamiliarPetDeedTemplate() {

	}

	~FamiliarPetDeedTemplate() {

	}

	void readObject(LuaObject* templateData) {
		DeedTemplate::readObject(templateData);
		controlDeviceObjectTemplate = templateData->getStringField("controlDeviceObjectTemplate");
		mobileTemplate = templateData->getStringField("mobileTemplate");
    }

	String getControlDeviceObjectTemplate() {
		return controlDeviceObjectTemplate;
	}

	String getMobileTemplate() {
		return mobileTemplate;
	}

};


#endif /* FamiliarPetDeedTemplate_H_ */
