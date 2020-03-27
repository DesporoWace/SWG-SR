/*
 * DuplicatorHintDataComponent.h
 *
 *  Created on: 11/8/2014
 *      Author: Tyclo
 */

#ifndef DUPLICATORHINTDATACOMPONENT_H_
#define DUPLICATORHINTDATACOMPONENT_H_

#include "server/zone/objects/scene/components/DataObjectComponent.h"

class DuplicatorHintDataComponent : public DataObjectComponent {
protected:
	int id;
	int group;
public:
	DuplicatorHintDataComponent() {

		id = -1;
		group = -1;
		addSerializableVariables();
	}

	virtual ~DuplicatorHintDataComponent() {

	}

	void writeJSON(nlohmann::json& j) const {
		DataObjectComponent::writeJSON(j);

		SERIALIZE_JSON_MEMBER(id);
		SERIALIZE_JSON_MEMBER(group);
	}

	void setId(int idNum) {
		id = idNum;
	}

	void setGroup(int groupNum) {
		group = groupNum;
	}

	int getId() const {
		return id;
	}

	int getGroup() const {
		return group;
	}

	bool isDuplicatorHintData() {
		return true;
	}

private:
	void addSerializableVariables() {
		addSerializableVariable("id", &id);
		addSerializableVariable("group", &group);
	}
};


#endif /* DUPLICATORHINTDATACOMPONENT_H_ */
