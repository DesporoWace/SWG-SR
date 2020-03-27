/*
 * DataDiskDataComponent.h
 *
 *  Created on: 25/6/2019
 *      Author: Tyclo
 */

#ifndef DATADISKDATACOMPONENT_H_
#define DATADISKDATACOMPONENT_H_

#include "server/zone/objects/scene/components/DataObjectComponent.h"

class DataDiskDataComponent : public DataObjectComponent {
protected:
	int unlockCode;

public:
	DataDiskDataComponent() {
		unlockCode = -1;
		addSerializableVariables();
	}

	virtual ~DataDiskDataComponent() {

	}

	void writeJSON(nlohmann::json& j) const {
		DataObjectComponent::writeJSON(j);

		SERIALIZE_JSON_MEMBER(unlockCode);
	}

	void setUnlockCode(int num) {
		unlockCode = num;
	}

	int getUnlockCode() const {
		return unlockCode;
	}

	bool isDataDiskData() {
		return true;
	}

private:
	void addSerializableVariables() {
		addSerializableVariable("unlockCode", &unlockCode);
	}
};


#endif /* DATADISKDATACOMPONENT_H_ */
