/*
 * DataDiskTemplate.h
 *
 *  Created on: 25/6/2019
 *      Author: Tyclo
 */

#ifndef DATADISKTEMPLATE_H_
#define DATADISKTEMPLATE_H_

#include "templates/SharedTangibleObjectTemplate.h"

class DataDiskTemplate: public SharedTangibleObjectTemplate {
protected:

	int unlockCode;
	int pages;
	Vector<String> waypoint;

public:
	DataDiskTemplate() {
		unlockCode = -1;
		pages = -1;
		waypoint;
	}

	~DataDiskTemplate() {

	}

	void readObject(LuaObject* templateData) {
		SharedTangibleObjectTemplate::readObject(templateData);
		unlockCode = templateData->getIntField("unlockCode");
		pages = templateData->getIntField("pages");

		LuaObject waypointData = templateData->getObjectField("waypoint");
		for (int i = 1; i <= waypointData.getTableSize(); ++i) {
			waypoint.add(waypointData.getStringAt(i));
		}
		waypointData.pop();
	}

	int getUnlockCode() {
		return unlockCode;
	}

	int getPages() {
		return pages;
	}

	Vector<String> getWaypoint() const {
		return waypoint;
	}

	bool isWaypointSet() {
		return waypoint.size() != 0;
	}

	bool isDataDiskTemplate() {
		return true;
	}

};

#endif /* DATADISKTEMPLATE_H_ */
