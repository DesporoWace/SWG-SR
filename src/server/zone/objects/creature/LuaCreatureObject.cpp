/*
 * LuaCreatureObject.cpp
 *
 *  Created on: 31/08/2009
 *      Author: victor
 */

#include "LuaCreatureObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/ai/DroidObject.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/objects/player/sessions/ConversationSession.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/player/sessions/EntertainingSession.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "server/zone/objects/intangible/PetControlDevice.h"

const char LuaCreatureObject::className[] = "LuaCreatureObject";

Luna<LuaCreatureObject>::RegType LuaCreatureObject::Register[] = {
		{ "_setObject", &LuaCreatureObject::_setObject },
		{ "_getObject", &LuaSceneObject::_getObject },
		{ "getBankCredits", &LuaCreatureObject::getBankCredits },
		{ "setBankCredits", &LuaCreatureObject::setBankCredits },
		{ "sendSystemMessage", &LuaCreatureObject::sendSystemMessage },
		{ "sendSystemMessageWithDI", &LuaCreatureObject::sendSystemMessageWithDI },
		{ "sendSystemMessageWithTO", &LuaCreatureObject::sendSystemMessageWithTO },
		{ "sendGroupMessage", &LuaCreatureObject::sendGroupMessage },
		{ "playMusicMessage", &LuaCreatureObject::playMusicMessage },
		{ "sendNewbieTutorialRequest", &LuaCreatureObject::sendNewbieTutorialRequest },
		{ "hasScreenPlayState", &LuaCreatureObject::hasScreenPlayState },
		{ "setScreenPlayState", &LuaCreatureObject::setScreenPlayState },
		{ "getScreenPlayState", &LuaCreatureObject::getScreenPlayState },
		{ "playEffect", &LuaSceneObject::playEffect },
		{ "sendNewbieTutorialEnableHudElement", &LuaCreatureObject::sendNewbieTutorialEnableHudElement },
		{ "getInCellNumber", &LuaCreatureObject::getInCellNumber },
		{ "getBuildingParentID", &LuaCreatureObject::getBuildingParentID },
		{ "sendOpenHolocronToPageMessage", &LuaCreatureObject::sendOpenHolocronToPageMessage },
		{ "inflictDamage", &LuaCreatureObject::inflictDamage },
		{ "setHAM", &LuaCreatureObject::setHAM },
		{ "setBaseHAM", &LuaCreatureObject::setBaseHAM },
		{ "setMaxHAM", &LuaCreatureObject::setMaxHAM },
		{ "getHAM", &LuaCreatureObject::getHAM },
		{ "getMaxHAM", &LuaCreatureObject::getMaxHAM },
		{ "getTargetID", &LuaCreatureObject::getTargetID },
		{ "clearCombatState", &LuaCreatureObject::clearCombatState },
		{ "getParent", &LuaSceneObject::getParent },
		{ "getZoneName", &LuaSceneObject::getZoneName },
		{ "getObjectID", &LuaSceneObject::getObjectID },
		{ "getPositionX", &LuaSceneObject::getPositionX },
		{ "getPositionY", &LuaSceneObject::getPositionY },
		{ "getPositionZ", &LuaSceneObject::getPositionZ },
		{ "getDirectionAngle", &LuaSceneObject::getDirectionAngle },
		{ "getWorldPositionX", &LuaSceneObject::getWorldPositionX },
		{ "getWorldPositionY", &LuaSceneObject::getWorldPositionY },
		{ "getWorldPositionZ", &LuaSceneObject::getWorldPositionZ },
		{ "getParentID", &LuaSceneObject::getParentID },
		{ "isInRangeWithObject", &LuaSceneObject::isInRangeWithObject },
		{ "getDistanceTo", &LuaSceneObject::getDistanceTo },
		{ "getServerObjectCRC", &LuaSceneObject::getServerObjectCRC },
		{ "isFeigningDeath", &LuaCreatureObject::isFeigningDeath},
		{ "hasState", &LuaCreatureObject::hasState},
		{ "setState", &LuaCreatureObject::setState},
		{ "clearState", &LuaCreatureObject::clearState},
		{ "setLootRights", &LuaCreatureObject::setLootRights},
		{ "getPosture", &LuaCreatureObject::getPosture},
		{ "setPosture", &LuaCreatureObject::setPosture},
		{ "setMoodString", &LuaCreatureObject::setMoodString},
		{ "hasSkill", &LuaCreatureObject::hasSkill},
		{ "removeSkill", &LuaCreatureObject::removeSkill},
		{ "surrenderSkill", &LuaCreatureObject::surrenderSkill},
		{ "getConversationSession", &LuaCreatureObject::getConversationSession},
		{ "doAnimation", &LuaCreatureObject::doAnimation},
		{ "engageCombat", &LuaCreatureObject::engageCombat},
		{ "getPlayerObject", &LuaCreatureObject::getPlayerObject},
		{ "setCustomObjectName", &LuaSceneObject::setCustomObjectName},
		{ "getFaction", &LuaCreatureObject::getFaction},
		{ "setFaction", &LuaCreatureObject::setFaction},
		{ "isRebel", &LuaCreatureObject::isRebel},
		{ "isImperial", &LuaCreatureObject::isImperial},
		{ "isNeutral", &LuaCreatureObject::isNeutral},
		{ "teleport", &LuaSceneObject::teleport},
		{ "getFirstName", &LuaCreatureObject::getFirstName},
		{ "getLastName", &LuaCreatureObject::getLastName},
		{ "setLastName", &LuaCreatureObject::setLastName},
		{ "isAiAgent", &LuaCreatureObject::isAiAgent},
		{ "setFactionRank", &LuaCreatureObject::setFactionRank},
		{ "getFactionRank", &LuaCreatureObject::getFactionRank},
		{ "getCashCredits", &LuaCreatureObject::getCashCredits},
		{ "subtractCashCredits", &LuaCreatureObject::subtractCashCredits},
		{ "addCashCredits", &LuaCreatureObject::addCashCredits},
		{ "removeScreenPlayState", &LuaCreatureObject::removeScreenPlayState},
		{ "isGrouped", &LuaCreatureObject::isGrouped},
		{ "isGroupedWith", &LuaCreatureObject::isGroupedWith},
		{ "getGroupSize", &LuaCreatureObject::getGroupSize},
		{ "getGroupMember", &LuaCreatureObject::getGroupMember},
		{ "setOptionsBitmask", &LuaCreatureObject::setOptionsBitmask},
		{ "setOptionBit", &LuaTangibleObject::setOptionBit},
		{ "clearOptionBit", &LuaTangibleObject::clearOptionBit},
		{ "setPvpStatusBitmask", &LuaTangibleObject::setPvpStatusBitmask},
		{ "setPvpStatusBit", &LuaTangibleObject::setPvpStatusBit},
		{ "isChangingFactionStatus", &LuaTangibleObject::isChangingFactionStatus },
		{ "setFutureFactionStatus", &LuaTangibleObject::setFutureFactionStatus },
		{ "addDotState", &LuaCreatureObject::addDotState},
		{ "getSlottedObject", &LuaSceneObject::getSlottedObject},
		{ "checkCooldownRecovery", &LuaCreatureObject::checkCooldownRecovery},
		{ "addCooldown", &LuaCreatureObject::addCooldown},
		{ "isDead", &LuaCreatureObject::isDead},
		{ "isIncapacitated", &LuaCreatureObject::isIncapacitated },
		{ "getLevel", &LuaCreatureObject::getLevel},
		{ "getQueueSize", &LuaCreatureObject::getQueueSize },
		{ "isDancing", &LuaCreatureObject::isDancing},
		{ "isPlayingMusic", &LuaCreatureObject::isPlayingMusic},
		{ "getPerformanceName", &LuaCreatureObject::getPerformanceName},
		{ "getWalkSpeed", &LuaCreatureObject::getWalkSpeed },
		{ "isAttackableBy", &LuaCreatureObject::isAttackableBy },
		{ "getSpecies", &LuaCreatureObject::getSpecies },
		{ "isDroidPet", &LuaCreatureObject::isDroidPet },
		{ "isCombatDroidPet", &LuaCreatureObject::isCombatDroidPet },
		{ "awardExperience", &LuaCreatureObject::awardExperience },
		{ "getOwner", &LuaCreatureObject::getOwner },
		{ "getCurrentSpeed", &LuaCreatureObject::getCurrentSpeed },
		{ "isInvisible", &LuaTangibleObject::isInvisible },
		{ "isInCombat", &LuaCreatureObject::isInCombat },
		{ "healDamage", &LuaCreatureObject::healDamage },
		{ "getGroupID", &LuaCreatureObject::getGroupID },
		{ "enhanceCharacter", &LuaCreatureObject::enhanceCharacter },
		{ "setWounds", &LuaCreatureObject::setWounds },
		{ "setShockWounds", &LuaCreatureObject::setShockWounds },
		{ "getForceSensitiveSkillCount", &LuaCreatureObject::getForceSensitiveSkillCount },
		{ "villageKnightPrereqsMet", &LuaCreatureObject::villageKnightPrereqsMet },
		{ "isOnLeave", &LuaTangibleObject::isOnLeave },
		{ "isOvert", &LuaTangibleObject::isOvert },
		{ "isCovert", &LuaTangibleObject::isCovert },
		{ "setFactionStatus", &LuaTangibleObject::setFactionStatus },
		{ "getDamageDealerList", &LuaCreatureObject::getDamageDealerList },
		{ "getHealingThreatList", &LuaCreatureObject::getHealingThreatList },
		{ "getSkillMod", &LuaCreatureObject::getSkillMod },
		{ "getGender", &LuaCreatureObject::getGender },
		{ "buffStat", &LuaCreatureObject::buffStat },
		{ "removeBuffs", &LuaCreatureObject::removeBuffs },
		{ "emptyStomach", &LuaCreatureObject::emptyStomach },
		{ "getActivePetsSize", &LuaCreatureObject::getActivePetsSize },
		{ "getActivePet", &LuaCreatureObject::getActivePet },
		{ "setAppearance", &LuaCreatureObject::setAppearance },

	// Mindsoft Added
		{ "getGroup", &LuaCreatureObject::getGroup },
		//{ "getGroupLock", &LuaCreatureObject::getGroupLock }, // TODO needs group locked variable and get/set methods added to group object idl, and observers/traps in group implementation
		//{ "setGroupLock", &LuaCreatureObject::setGroupLock }, // TODO needs group locked variable and get/set methods added to group object idl, and observers/traps in group implementation
		{ "formGroupWithCreature", &LuaCreatureObject::formGroupWithCreature },
		{ "ungroupCreature", &LuaCreatureObject::ungroupCreature },
		{ "clearDots", &LuaCreatureObject::clearDots},
		//{ "buffCharacter", &LuaCreatureObject::buffCharacter }, //TODO Research how to implement this here
		{ "removeBuff", &LuaCreatureObject::removeBuff },
		{ "getActiveThreatList", &LuaCreatureObject::getActiveThreatList },
		{ "getThreatMap", &LuaCreatureObject::getThreatMap },
		{ "clearTargetAggro", &LuaCreatureObject::clearTargetAggro },
		{ "setHeight", &LuaCreatureObject::setHeight },
		{ "getCooldownTime", &LuaCreatureObject::getCooldownTime },
		{ "getCooldownString", &LuaCreatureObject::getCooldownString },
		{ "sendCommand", &LuaCreatureObject::sendCommand },
	// End Mindsoft Adds
		{ "getPetControlDevice", &LuaCreatureObject::getPetControlDevice }, // Tyclo
		{ 0, 0 }
};

LuaCreatureObject::LuaCreatureObject(lua_State *L) : LuaTangibleObject(L) {
#ifdef DYNAMIC_CAST_LUAOBJECTS
	realObject = dynamic_cast<CreatureObject*>(_getRealSceneObject());

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	realObject = static_cast<CreatureObject*>(lua_touserdata(L, 1));
#endif
}

LuaCreatureObject::~LuaCreatureObject(){
}

int LuaCreatureObject::_setObject(lua_State* L) {
	LuaTangibleObject::_setObject(L);

#ifdef DYNAMIC_CAST_LUAOBJECTS
	auto obj = dynamic_cast<CreatureObject*>(_getRealSceneObject());

	if (obj != realObject)
		realObject = obj;

	assert(!_getRealSceneObject() || realObject != NULL);
#else
	auto obj = static_cast<CreatureObject*>(lua_touserdata(L, -1));

	if (realObject != obj)
		realObject = obj;
#endif

	return 0;
}

int LuaCreatureObject::getFirstName(lua_State* L) {
	String text = realObject->getFirstName();
	lua_pushstring(L, text.toCharArray());
	return 1;
}

int LuaCreatureObject::getLastName(lua_State* L) {
	String text = realObject->getLastName();
	lua_pushstring(L, text.toCharArray());
	return 1;
}

int LuaCreatureObject::setLastName(lua_State* L) {
	int argc = lua_gettop(L) - 1;

	if (argc < 1 || argc > 2) {
		Logger::console.error("incorrect number of arguments for LuaCreatureObject::setLastName");
		return 0;
	}

	String newLastName;
	bool skipVerify;

	if (argc == 1) {
		newLastName = lua_tostring(L, -1);
		skipVerify = false;
	} else {
		newLastName = lua_tostring(L, -2);
		skipVerify = lua_toboolean(L, -1);
	}

	Locker locker(realObject);

	auto errmsg = realObject->setLastName(newLastName, skipVerify);

	lua_pushstring(L, errmsg.toCharArray());

	return 1;
}

	int LuaCreatureObject::addDotState(lua_State* L) {
	//addDotState(unsigned long dotType, unsigned int strength, byte type, unsigned int duration, float potency, unsigned int defense)
	uint32 defense = lua_tointeger(L, -1);
	uint32 objectID = lua_tointeger(L, -2);
	float potency = lua_tonumber(L, -3);
	uint32 duration = lua_tointeger(L, -4);
	byte type = lua_tointeger(L, -5);
	uint32 strength = lua_tointeger(L, -6);
	uint64 dotType = lua_tointeger(L, -7);
	CreatureObject* attacker = (CreatureObject*)lua_touserdata(L, -8);

	Locker locker(realObject);

	realObject->addDotState(attacker, dotType, objectID, strength, type, duration, potency, defense);

	return 0;
}

int LuaCreatureObject::isFeigningDeath(lua_State* L) {
	lua_pushnumber(L, realObject->isFeigningDeath());
	return 1;
}

int LuaCreatureObject::hasState(lua_State* L) {
	uint32 state = (uint32) lua_tonumber(L, -1);

	lua_pushnumber(L, realObject->hasState(state));
	return 1;
}

int LuaCreatureObject::setState(lua_State* L) {
	uint32 state = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->setState(state, true);

	return 0;
}

int LuaCreatureObject::clearState(lua_State* L) {
	uint32 state = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->clearState(state, true);

	return 0;
}

int LuaCreatureObject::getPosture(lua_State* L) {
	lua_pushnumber(L, realObject->getPosture());

	return 1;
}

int LuaCreatureObject::setPosture(lua_State* L) {
	uint32 posture = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->setPosture(posture, true);

	return 0;
}

int LuaCreatureObject::setMoodString(lua_State* L) {
	String value = lua_tostring(L, -1);

	Locker locker(realObject);

	realObject->setMoodString(value);

	return 0;
}

int LuaCreatureObject::sendOpenHolocronToPageMessage(lua_State* L) {
	realObject->sendOpenHolocronToPageMessage();

	return 0;
}

int LuaCreatureObject::sendSystemMessage(lua_State* L) {
	if (lua_islightuserdata(L, -1)) {
		StringIdChatParameter* message = (StringIdChatParameter*)lua_touserdata(L, -1);
		realObject->sendSystemMessage(*message);
	} else {
		String value = lua_tostring(L, -1);
		realObject->sendSystemMessage(value);
	}
	return 0;
}

int LuaCreatureObject::sendSystemMessageWithDI(lua_State* L) {
	String text = lua_tostring(L, -2);
	uint32 value = (uint32) lua_tonumber(L, -1);

	StringIdChatParameter param(text);
	param.setDI(value);

	realObject->sendSystemMessage(param);

	return 0;
}

int LuaCreatureObject::sendSystemMessageWithTO(lua_State* L) {
	String text = lua_tostring(L, -2);
	String value = lua_tostring(L, -1);

	StringIdChatParameter param(text);
	param.setTO(value);

	realObject->sendSystemMessage(param);

	return 0;
}

int LuaCreatureObject::sendSystemMessageWithTT(lua_State* L) {
	String text = lua_tostring(L, -2);
	String value = lua_tostring(L, -1);

	StringIdChatParameter param(text);
	param.setTT(value);

	realObject->sendSystemMessage(param);

	return 0;
}

int LuaCreatureObject::sendGroupMessage(lua_State* L) {
	String value = lua_tostring(L, -1);

	if (realObject == NULL)
		return 0;

	if (!realObject->isGrouped()) {
		realObject->sendSystemMessage(value);
	} else {
		GroupObject* group = realObject->getGroup();

		group->sendSystemMessage(value);
	}

	return 0;
}

int LuaCreatureObject::setHAM(lua_State* L) {
	uint32 type = (uint32) lua_tonumber(L, -2);
	uint32 value = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->setHAM(type, value);

	return 0;
}

int LuaCreatureObject::setBaseHAM(lua_State* L) {
	uint32 type = (uint32) lua_tonumber(L, -2);
	uint32 value = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->setBaseHAM(type, value);

	return 0;
}

int LuaCreatureObject::setMaxHAM(lua_State* L) {
	uint32 type = (uint32) lua_tonumber(L, -2);
	uint32 value = (uint32) lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->setMaxHAM(type, value);

	return 0;
}

int LuaCreatureObject::playMusicMessage(lua_State *L) {
	String value = lua_tostring(L, -1);
	realObject->playMusicMessage(value);

	return 0;
}

int LuaCreatureObject::setBankCredits(lua_State *L) {
	uint32 credits = (uint32) lua_tonumber(L, -1);

	realObject->setBankCredits(credits);

	return 0;
}

int LuaCreatureObject::sendNewbieTutorialRequest(lua_State *L) {
	String value = lua_tostring(L, -1);
	realObject->sendNewbieTutorialRequest(value);

	return 0;
}

int LuaCreatureObject::hasScreenPlayState(lua_State *L) {
	String play = lua_tostring(L, -1);
	uint64 stateToCheck = lua_tointeger(L, -2);

	uint64 state = realObject->getScreenPlayState(play);

	bool check = state & stateToCheck;

	lua_pushboolean(L, check);

	return 1;
}

int LuaCreatureObject::getScreenPlayState(lua_State *L) {
	String play = lua_tostring(L, -1);
	uint64 state = realObject->getScreenPlayState(play);

	lua_pushinteger(L, state);

	return 1;
}

int LuaCreatureObject::sendNewbieTutorialEnableHudElement(lua_State* L) {
	String ui = lua_tostring(L, -3);
	int enable = lua_tonumber(L, -2);
	float blinkCount = lua_tonumber(L, -1);

	realObject->sendNewbieTutorialEnableHudElement(ui, enable, blinkCount);

	return 0;
}

int LuaCreatureObject::hasSkill(lua_State* L) {
	String value = lua_tostring(L, -1);

	bool check = realObject->hasSkill(value);

	lua_pushboolean(L, check);

	return 1;
}

int LuaCreatureObject::removeSkill(lua_State* L) {
	String value = lua_tostring(L, -1);

	Locker locker(realObject);

	realObject->removeSkill(value, true);
	return 0;
}

int LuaCreatureObject::surrenderSkill(lua_State* L) {
	String value = lua_tostring(L, -1);

	SkillManager* skillManager = SkillManager::instance();
	skillManager->surrenderSkill(value, realObject, true);
	return 0;
}


int LuaCreatureObject::getInCellNumber(lua_State* L) {
	SceneObject* parent = realObject->getParent().get().get();

	if (parent == NULL || !parent->isCellObject())
		lua_pushnumber(L, -1);
	else {
		int cellId = ((CellObject*)parent)->getCellNumber();

		lua_pushnumber(L, cellId);
	}

	return 1; // we return the number of vars pushed to the L stack
}

int LuaCreatureObject::getBuildingParentID(lua_State* L) {
	ManagedReference<SceneObject*> parent = realObject->getParentRecursively(SceneObjectType::BUILDING);

	if (parent == NULL)
		lua_pushnumber(L, 0);
	else
		lua_pushnumber(L, parent->getObjectID());

	return 1;
}

int LuaCreatureObject::setScreenPlayState(lua_State *L) {
	String play = lua_tostring(L, -1);
	uint64 stateToSet = lua_tointeger(L, -2);

	realObject->setScreenPlayState(play, stateToSet | realObject->getScreenPlayState(play));
	realObject->notifyObservers(ObserverEventType::SCREENPLAYSTATECHANGED, realObject);
	return 0;
}

int LuaCreatureObject::removeScreenPlayState(lua_State* L) {
	String play = lua_tostring(L, -1);
	uint64 stateToClear = lua_tointeger(L, -2);

	if (realObject != NULL) {
		realObject->setScreenPlayState(play, realObject->getScreenPlayState(play) & (~stateToClear));
		realObject->notifyObservers(ObserverEventType::SCREENPLAYSTATECHANGED, realObject);
	}
	return 0;
}

int LuaCreatureObject::getTargetID(lua_State* L) {
	uint64 targetID = realObject->getTargetID();

	lua_pushinteger(L, targetID);

	return 1;
}

int LuaCreatureObject::clearCombatState(lua_State* L) {
	bool clearDef = lua_tonumber(L, -1);

	Locker locker(realObject);

	realObject->clearCombatState(clearDef);

	return 0;
}

int LuaCreatureObject::getHAM(lua_State* L) {
	int type = lua_tonumber(L, -1);

	int value = realObject->getHAM(type);

	lua_pushnumber(L, value);

	return 1;
}

int LuaCreatureObject::getMaxHAM(lua_State* L) {
	int type = lua_tonumber(L, -1);

	int value = realObject->getMaxHAM(type);

	lua_pushnumber(L, value);

	return 1;
}

int LuaCreatureObject::inflictDamage(lua_State* L) {
 //int inflictDamage(TangibleObject attacker, int damageType, int damage, boolean destroy, boolean notifyClient = true);
	int destroy = lua_tonumber(L, -1);
	int damage = lua_tonumber(L, -2);
	int damageType = lua_tonumber(L, -3);

	SceneObject* scene = (SceneObject*) lua_touserdata(L, -4);

	TangibleObject* attacker = cast<TangibleObject*>(scene);

	assert(attacker);

	Locker locker(realObject);

	Locker crossLocker(attacker, realObject);

	realObject->inflictDamage(attacker, damageType, damage, destroy);

	return 0;
}

int LuaCreatureObject::getBankCredits(lua_State *L) {
	lua_pushnumber(L, realObject->getBankCredits());

	return 1; // we return the number of vars pushed to the L stack
}

int LuaCreatureObject::getConversationSession(lua_State* L) {
	Reference<ConversationSession*> session = realObject->getActiveSession(SessionFacadeType::CONVERSATION).castTo<ConversationSession*>();

	if (session != NULL) {
		session->_setUpdated(true);
		lua_pushlightuserdata(L, session);
	} else
		lua_pushnil(L);

	return 1;
}

int LuaCreatureObject::doAnimation(lua_State* L) {
	String animString = lua_tostring(L, -1);

	if (realObject != NULL)
		realObject->doAnimation(animString);

	return 0;
}

int LuaCreatureObject::engageCombat(lua_State* L) {
	CreatureObject* enemy = (CreatureObject*)lua_touserdata(L, -1);

	Locker locker(realObject);

	if (enemy != NULL)
		realObject->addDefender(enemy);

	return 0;
}

int LuaCreatureObject::getPlayerObject(lua_State* L) {
	Reference<PlayerObject*> obj = realObject->getPlayerObject();

	if (obj != NULL) {
		obj->_setUpdated(true);
		lua_pushlightuserdata(L, obj);
	} else
		lua_pushnil(L);

	return 1;
}

int LuaCreatureObject::getFaction(lua_State* L) {
	uint32 faction = realObject->getFaction();

	lua_pushinteger(L, faction);

	return 1;
}

int LuaCreatureObject::isRebel(lua_State* L) {
	lua_pushboolean(L, realObject->isRebel());

	return 1;
}

int LuaCreatureObject::isNeutral(lua_State* L) {
	lua_pushboolean(L, realObject->isNeutral());

	return 1;
}

int LuaCreatureObject::isImperial(lua_State* L) {
	lua_pushboolean(L, realObject->isImperial());

	return 1;
}

int LuaCreatureObject::setFaction(lua_State* L) {
	uint32 faction = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setFaction(faction);

	return 0;
}

int LuaCreatureObject::setFactionRank(lua_State* L) {
	int rank = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setFactionRank(rank);

	return 0;
}

int LuaCreatureObject::getFactionRank(lua_State* L) {
	lua_pushinteger(L, realObject->getFactionRank());

	return 1;
}

int LuaCreatureObject::getCashCredits(lua_State* L) {
	lua_pushinteger(L, realObject->getCashCredits());

	return 1;
}

int LuaCreatureObject::subtractCashCredits(lua_State* L) {
	Locker locker(realObject);

	realObject->subtractCashCredits(lua_tointeger(L, -1));

	return 0;
}

int LuaCreatureObject::addCashCredits(lua_State* L) {
	bool notifyClient = lua_toboolean(L, -1);
	int credits = lua_tonumber(L, -2);

	Locker locker(realObject);

	realObject->addCashCredits(credits, notifyClient);

	return 0;
}

int LuaCreatureObject::isAiAgent(lua_State* L) {
	bool val = realObject->isAiAgent();

	lua_pushboolean(L, val);

	return 1;
}

int LuaCreatureObject::isGrouped(lua_State* L) {
	bool val = realObject->isGrouped();

	lua_pushboolean(L, val);

	return 1;
}

int LuaCreatureObject::isGroupedWith(lua_State* L) {
	CreatureObject* groupMember = (CreatureObject*) lua_touserdata(L, -1);

	if (realObject == NULL || groupMember == NULL || !realObject->isGrouped())
		return 0;

	GroupObject* group = realObject->getGroup();

	lua_pushboolean(L, group != NULL && group->hasMember(groupMember));

	return 1;
}

int LuaCreatureObject::setLootRights(lua_State* L) {
	CreatureObject* player = (CreatureObject*) lua_touserdata(L, -1);

	if (realObject == NULL)
		return 0;

	uint64 ownerID = 0;

	if (player != NULL) {
		ownerID = player->getObjectID();
	}

	SceneObject* inventory = realObject->getSlottedObject("inventory");

	if (inventory == NULL)
		return 0;

	Locker locker(inventory);

	inventory->setContainerOwnerID(ownerID);
	return 1;
}

int LuaCreatureObject::getGroupSize(lua_State* L) {
	Locker locker(realObject);

	GroupObject* group = realObject->getGroup();

	if (group == NULL) {
		lua_pushnumber(L, 0);
		return 1;
	}

	Locker lock(group, realObject);

	lua_pushnumber(L, group->getGroupSize());

	return 1;
}

int LuaCreatureObject::setOptionsBitmask(lua_State* L) {
	uint32 bitmask = lua_tointeger(L, -1);

	Locker locker(realObject);

	realObject->setOptionsBitmask(bitmask, true);

	return 0;
}

int LuaCreatureObject::getGroupMember(lua_State* L) {
	int i = lua_tonumber(L, -1);

	if (i < 0)
		i = 0;

	Locker locker(realObject);

	GroupObject* group = realObject->getGroup();

	if (group == NULL) {
		lua_pushnil(L);
		return 1;
	}

	Locker lock(group, realObject);

	if (group->getGroupSize() < i) {
		lua_pushnil(L);
		return 1;
	}

	CreatureObject* creo = group->getGroupMember(i);

	if (creo == NULL) {
		realObject->info("LuaCreatureObject::getGroupMember GroupMember is NULL.");
		lua_pushnil(L);
	} else {
		creo->_setUpdated(true);
		lua_pushlightuserdata(L, creo);
	}

	return 1;
}

int LuaCreatureObject::checkCooldownRecovery(lua_State* L) {
	String cooldownName = lua_tostring(L, -1);

	lua_pushboolean(L, realObject->checkCooldownRecovery(cooldownName));

	return 1;
}

int LuaCreatureObject::addCooldown(lua_State* L) {
	String cooldownName = lua_tostring(L, -2);
	int milliseconds = lua_tointeger(L, -1);

	realObject->addCooldown(cooldownName, milliseconds);

	return 0;
}

int LuaCreatureObject::isDead(lua_State* L) {
	bool retVal = realObject->isDead();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaCreatureObject::isIncapacitated(lua_State* L) {
	lua_pushboolean(L, realObject->isIncapacitated());
	return 1;
}

int LuaCreatureObject::getLevel(lua_State* L) {
	int level = realObject->getLevel();

	lua_pushinteger(L, level);

	return 1;
}

int LuaCreatureObject::getQueueSize(lua_State* L) {
	int size = realObject->getCommandQueueSize();

	lua_pushinteger(L, size);

	return 1;
}

int LuaCreatureObject::isDancing(lua_State* L) {
	bool retVal = realObject->isDancing();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaCreatureObject::isPlayingMusic(lua_State* L) {
	bool retVal = realObject->isPlayingMusic();

	lua_pushboolean(L,  retVal);

	return 1;
}

int LuaCreatureObject::getPerformanceName(lua_State* L) {
	ManagedReference<Facade*> facade = realObject->getActiveSession(SessionFacadeType::ENTERTAINING);

	if (facade == NULL) {
		lua_pushnil(L);
		return 1;
	}

	ManagedReference<EntertainingSession*> session = dynamic_cast<EntertainingSession*> (facade.get());

	if (session == NULL) {
		lua_pushnil(L);
		return 1;
	}

	if (!session->isPlayingMusic() && !session->isDancing())
		lua_pushnil(L);
	else
		lua_pushstring(L, session->getPerformanceName().toCharArray());

	return 1;
}

int LuaCreatureObject::getWalkSpeed(lua_State* L) {
	lua_pushnumber(L, realObject->getWalkSpeed());
	return 1;
}

int LuaCreatureObject::isAttackableBy(lua_State* L) {
	TangibleObject* obj = (TangibleObject*) lua_touserdata(L, -1);

	bool retVal = realObject->isAttackableBy(obj);
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaCreatureObject::getSpecies(lua_State* L) {
	lua_pushinteger(L, realObject->getSpecies());

	return 1;
}

int LuaCreatureObject::isDroidPet(lua_State* L) {
	bool retVal = realObject->isDroidObject() && realObject->isPet();
	lua_pushboolean(L, retVal);

	return 1;
}

int LuaCreatureObject::isCombatDroidPet(lua_State* L) {
	bool retVal = false;
	if (realObject->isDroidObject() && realObject->isPet()) {
		DroidObject* d = cast<DroidObject*>(realObject);
		retVal = d->isCombatDroid();
	}
	lua_pushboolean(L, retVal);
	return 1;
}

int LuaCreatureObject::awardExperience(lua_State* L) {
	String experienceType = lua_tostring(L, -3);
	int experienceAmount = lua_tointeger(L, -2);
	bool sendSysMessage = lua_toboolean(L, -1);

	PlayerManager* playerManager = realObject->getZoneServer()->getPlayerManager();
	playerManager->awardExperience(realObject, experienceType, experienceAmount, sendSysMessage, 1.0f, false);

	return 0;
}

int LuaCreatureObject::getOwner(lua_State* L) {
	CreatureObject* retVal = realObject->getLinkedCreature().get();

	if (retVal == NULL)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, retVal);
	return 1;
}

int LuaCreatureObject::getCurrentSpeed(lua_State* L) {
	float currentSpeed = realObject->getCurrentSpeed();

	lua_pushnumber(L, currentSpeed);

	return 1;
}

int LuaCreatureObject::isInCombat(lua_State* L) {
	bool retVal = realObject->isInCombat();

	lua_pushboolean(L, retVal);

	return 1;
}

int LuaCreatureObject::healDamage(lua_State* L) {
	int damageHealed = lua_tointeger(L, -2);
	int pool = lua_tointeger(L, -1);

	realObject->healDamage(realObject, pool, damageHealed, true, true);

	return 0;
}

int LuaCreatureObject::getGroupID(lua_State* L) {

	lua_pushnumber(L, realObject->getGroupID());

	return 1;
}

int LuaCreatureObject::enhanceCharacter(lua_State* L) {
	PlayerManager* playerManager = realObject->getZoneServer()->getPlayerManager();
	playerManager->enhanceCharacter(realObject);

	return 0;
}

int LuaCreatureObject::setWounds(lua_State* L) {
	int amount = lua_tointeger(L, -1);
	int pool = lua_tointeger(L, -2);

	realObject->setWounds(pool, amount, true);

	return 0;
}

int LuaCreatureObject::setShockWounds(lua_State* L) {
	int amount = lua_tointeger(L, -1);

	realObject->setShockWounds(amount, true);

	return 0;
}

int LuaCreatureObject::getForceSensitiveSkillCount(lua_State* L) {
	bool includeNoviceMasterBoxes = lua_toboolean(L, -1);

	int result = SkillManager::instance()->getForceSensitiveSkillCount(realObject, includeNoviceMasterBoxes);

	lua_pushnumber(L, result);

	return 1;
}

int LuaCreatureObject::villageKnightPrereqsMet(lua_State* L) {
	String skillToDrop = lua_tostring(L, -1);

	bool result = SkillManager::instance()->villageKnightPrereqsMet(realObject, skillToDrop);

	lua_pushboolean(L, result);

	return 1;
}

int LuaCreatureObject::getDamageDealerList(lua_State* L) {
	ThreatMap* threatMap = realObject->getThreatMap();
	ThreatMap copyThreatMap(*threatMap);

	lua_newtable(L);

	int count = 0;
	for (int i = 0; i < copyThreatMap.size(); ++i) {
		ThreatMapEntry* entry = &copyThreatMap.elementAt(i).getValue();

		if (entry->getTotalDamage() > 0) {
			CreatureObject* attacker = copyThreatMap.elementAt(i).getKey();

			count++;
			lua_pushlightuserdata(L, attacker);
			lua_rawseti(L, -2, count);
		}
	}

	return 1;
}

int LuaCreatureObject::getHealingThreatList(lua_State* L) {
	ThreatMap* threatMap = realObject->getThreatMap();
	ThreatMap copyThreatMap(*threatMap);

	lua_newtable(L);

	int count = 0;
	for (int i = 0; i < copyThreatMap.size(); ++i) {
		ThreatMapEntry* entry = &copyThreatMap.elementAt(i).getValue();

		if (entry->getHeal() > 0) {
			CreatureObject* healer = copyThreatMap.elementAt(i).getKey();

			count++;
			lua_pushlightuserdata(L, healer);
			lua_rawseti(L, -2, count);
		}
	}

	return 1;
}

int LuaCreatureObject::getSkillMod(lua_State* L) {
	String skillMod = lua_tostring(L, -1);

	int result = realObject->getSkillMod(skillMod);

	lua_pushnumber(L, result);

	return 1;
}

// buffSingleStat(String stat, int buffPower, int buffDuration)
int LuaCreatureObject::buffStat(lua_State* L) {
	String stat = lua_tostring(L, -3);
	int buffPower = lua_tointeger(L, -2);
	int buffDuration = lua_tointeger(L, -1);

	PlayerManager* playerManager = realObject->getZoneServer()->getPlayerManager();

	bool appliedBuff = true; // Because doEnhanceCharacter() is a bool function...

	if (stat == "HA"){
		appliedBuff = playerManager->doEnhanceCharacter(0x98321369, realObject, buffPower, buffDuration, BuffType::MEDICAL, 0); // medical_enhance_health
		appliedBuff = playerManager->doEnhanceCharacter(0x815D85C5, realObject, buffPower, buffDuration, BuffType::MEDICAL, 1); // medical_enhance_strength
		appliedBuff = playerManager->doEnhanceCharacter(0x7F86D2C6, realObject, buffPower, buffDuration, BuffType::MEDICAL, 2); // medical_enhance_constitution
		appliedBuff = playerManager->doEnhanceCharacter(0x4BF616E2, realObject, buffPower, buffDuration, BuffType::MEDICAL, 3); // medical_enhance_action
		appliedBuff = playerManager->doEnhanceCharacter(0x71B5C842, realObject, buffPower, buffDuration, BuffType::MEDICAL, 4); // medical_enhance_quickness
		appliedBuff = playerManager->doEnhanceCharacter(0xED0040D9, realObject, buffPower, buffDuration, BuffType::MEDICAL, 5); // medical_enhance_stamina
	} else if (stat == "M") {
		appliedBuff = playerManager->doEnhanceCharacter(0x11C1772E, realObject, buffPower, buffDuration, BuffType::PERFORMANCE, 6); // performance_enhance_dance_mind
		appliedBuff = playerManager->doEnhanceCharacter(0x2E77F586, realObject, buffPower, buffDuration, BuffType::PERFORMANCE, 7); // performance_enhance_music_focus
		appliedBuff = playerManager->doEnhanceCharacter(0x3EC6FCB6, realObject, buffPower, buffDuration, BuffType::PERFORMANCE, 8); // performance_enhance_music_willpower
	} else {
		appliedBuff = false;
	}

	return 0;
}

int LuaCreatureObject::removeBuffs(lua_State* L) {
	Reference<PlayerObject*> player = realObject->getPlayerObject();

	realObject->clearBuffs(true, false);

	return 0;
}

int LuaCreatureObject::emptyStomach(lua_State* L) {

	if (!realObject->isPlayerCreature())
		return 0;

	Reference<PlayerObject*> player = realObject->getPlayerObject();

	player->setFoodFilling(0);
	player->setDrinkFilling(0);

	return 0;
}

// Return the number of pets a player has out
int LuaCreatureObject::getActivePetsSize(lua_State* L) {
	ManagedReference<PlayerObject*> player = realObject->getPlayerObject();

	lua_pushinteger(L, player->getActivePetsSize());

	return 1;
}

// Return a pointer to the specified pet
int LuaCreatureObject::getActivePet(lua_State* L) {
	int petNumber = lua_tointeger(L, -1);

	petNumber = (0, petNumber);

	//Logger::console.info("petNumber: " + String::valueOf(petNumber), true);

	ManagedReference<PlayerObject*> player = realObject->getPlayerObject();
	ManagedReference<CreatureObject*> pet = player->getActivePet(petNumber);

	if (pet != NULL) {
		String petName = pet->getFirstName();
		//Logger::console.info("Pet Name: " + petName, true);
	}

	lua_pushlightuserdata(L, pet);

	return 1;
}

int LuaCreatureObject::getGender(lua_State* L) {

	lua_pushnumber(L, realObject->getGender());

	return 1;
}

int LuaCreatureObject::setAppearance(lua_State* L) {
	String templateName = lua_tostring(L, -1);

	realObject->setAlternateAppearance(templateName, true);

	// If a player is used they should reload their client, such as teleporting them to the same location. Lazzy and this is handled in the lua script.
	return 0;
}


// Mindsoft Added

// getGroup : retrieve creatures group object pointer
//  use: CreatureObject(pCreature):getGroup()
//  returns: group object pointer
int LuaCreatureObject::getGroup(lua_State* L) {
	if (realObject->isGrouped()) {
		GroupObject* group = realObject->getGroup();
		lua_pushlightuserdata(L, group);
	} else {
		lua_pushboolean(L, false);
	}

	return 1;
}

/*
// getGroupLock : retrieve lock status of creatures group
//  use: CreatureObject(pCreature):getGroupLock()
//  returns: bool ( true = locked , false = unlocked )
int LuaCreatureObject::getGroupLock(lua_State* L) { //Mindsoft added for controling groups
	if (realObject == NULL)
		return 0;

	if (!realObject->isGrouped()) {
		lua_pushboolean(L, false);
	} else {
		GroupObject* group = realObject->getGroup();
		lua_pushboolean(L, group->getGroupLock());
	}

	return 1;
}

// setGroupLock : attempt to lock creatures group
//  use: CreatureObject(pCreature):setGroupLock()
//  returns: nothing
int LuaCreatureObject::setGroupLock(lua_State* L) { //Mindsoft added for controling groups
	bool newState = lua_tonumber(L, -1);

	if (realObject == NULL)
		return 0;

	if (realObject->isGrouped()) {
		GroupObject* group = realObject->getGroup();
		Locker clock(realObject);
		Locker lock(group, realObject);
		group->setGroupLock(newState);
	}

	return 0;
}
*/

// formGroupWithCreature : attempt to group creature with target creature
//  use: CreatureObject(pCreature):formGroupWithCreature(pTarget)
//  returns: groupID
int LuaCreatureObject::formGroupWithCreature(lua_State* L) {
	CreatureObject* target = (CreatureObject*) lua_touserdata(L, -1);
	if (target->isGrouped()) {
		lua_pushnil(L);
	} else {
		GroupManager* groupManager = GroupManager::instance();
		Locker locker(target);
		target->updateGroupInviterID(realObject->getObjectID());
		groupManager->joinGroup(target);
		lua_pushnumber(L, realObject->getGroupID());
	}

	return 1;
}

// ungroupCreature : attempt to have creature leave group
//  use: CreatureObject(pCreature):ungroupCreature()
//  returns: nothing
int LuaCreatureObject::ungroupCreature(lua_State* L) {
	if (realObject->isGrouped()) {
		GroupManager* groupManager = GroupManager::instance();
		GroupObject* group = realObject->getGroup();
		Locker lock(realObject);
		groupManager->leaveGroup(group, realObject);
	}
	return 0;
}

// clearDots : clear all creature dots
//  use: CreatureObject(pCreature):clearDots()
//  returns: nothing
int LuaCreatureObject::clearDots(lua_State* L) {
	Locker locker(realObject);
	realObject->clearDots();
	return 0;
}

/*
// buffCharacter : apply buff on creature
//  use: CreatureObject(pCreature):buffCharacter(crc,amount,duration,buff-type,attribute,absorption)
//  returns: nothing
int LuaCreatureObject::buffCharacter(lua_State* L) {
	uint32 crc = lua_tointeger(L, -6);
	int amount = lua_tointeger(L, -5);
	int duration = lua_tointeger(L, -4);
	int buffType = lua_tointeger(L, -3);
	uint8 attribute = lua_tointeger(L, -2);
	int absorption = lua_tointeger(L, -1);

	PlayerManager* playerManager = realObject->getZoneServer()->getPlayerManager();
	playerManager->doEnhanceCharacter(crc, realObject, amount, duration, buffType, attribute, absorption); //TODO

	return 0;
}
*/

// removeBuff() : remove specified buff from creature
// use: CreatureObject(pCreature):removeBuff(buffString)
//returns healers & attackers in creature threat map --mindsoft added
int LuaCreatureObject::removeBuff(lua_State* L) {
	String buffToDrop = lua_tostring(L, -1);
	uint32 buffCRC = buffToDrop.hashCode();

	if (realObject->hasBuff(buffCRC)) {
		realObject->removeBuff(buffCRC);
	}
	return 0;
}

// getActiveThreatList : get creatures active threat list
//  use: CreatureObject(pCreature):getActiveThreatList()
//  returns: active healers & attackers in creature threat map
int LuaCreatureObject::getActiveThreatList(lua_State* L) {
	Locker locker(realObject);
	ThreatMap* threatMap = realObject->getThreatMap();
	ThreatMap copyThreatMap(*threatMap);

	lua_newtable(L);

	int count = 0;
	for (int i = 0; i < copyThreatMap.size(); ++i) {
		ThreatMapEntry* entry = &copyThreatMap.elementAt(i).getValue();

		if ((entry->getTotalDamage() > 0) || (entry->getHeal() > 0)) {
			CreatureObject* threat = copyThreatMap.elementAt(i).getKey();

			count++;
			lua_pushlightuserdata(L, threat);
			lua_rawseti(L, -2, count);
		}
	}

	return 1;
}

// getThreatMap : get creature passive and active threat lists
//  use: CreatureObject(pCreature):getThreatMap()
//  returns: entire creature threat map
int LuaCreatureObject::getThreatMap(lua_State* L) {
	Locker locker(realObject);
	ThreatMap* threatMap = realObject->getThreatMap();
	ThreatMap copyThreatMap(*threatMap);

	lua_newtable(L);

	int count = 0;
	for (int i = 0; i < copyThreatMap.size(); ++i) {
		CreatureObject* threat = copyThreatMap.elementAt(i).getKey();

		count++;
		lua_pushlightuserdata(L, threat);
		lua_rawseti(L, -2, count);
	}

	return 1;
}

// clearTargetAggro : removes targets threat from creature threat map
//  use: CreatureObject(pCreature):clearTargetAggro(pTarget)
//  returns: nothing
int LuaCreatureObject::clearTargetAggro(lua_State* L) {
	CreatureObject* target = (CreatureObject*) lua_touserdata(L, -1);
	ThreatMap* tMap = realObject->getThreatMap();
	tMap->clearAggro(target);
	return 0;
}

// setHeight : set creature height
//  use: CreatureObject(pCreature):setHeight(floatScale, boolNotifyClient)
//  returns: nothing
int LuaCreatureObject::setHeight(lua_State* L) {
	bool notifyClient = lua_toboolean(L, -1);
	float amount = lua_tonumber(L, -2);

	Locker locker(realObject);
	realObject->setHeight(amount, notifyClient);

	return 0;
}

// getCooldownTime :  get time integer of remaining cooldown specified in ms
//  use: CreatureObject(pCreature):getCooldownTime(cooldownString)
//  returns: integer time remaining on cooldown
int LuaCreatureObject::getCooldownTime(lua_State* L) {
	String cdString = lua_tostring(L, -1);
	Time* timeRemaining = realObject->getCooldownTime(cdString);
	uint32 result = (timeRemaining->miliDifference() * -1);
	lua_pushnumber(L, result);
	return 1;
}

// getCooldownString : get formatted time string of remaining cooldown specified
//  use: CreatureObject(pCreature):getCooldownString(cooldownString)
//  returns: formatted string time remaining on cooldown
int LuaCreatureObject::getCooldownString(lua_State* L) {
	String cdString = lua_tostring(L, -1);
	Time* timeRemaining = realObject->getCooldownTime(cdString);
	uint32 delta = (timeRemaining->miliDifference() * -1);

	int seconds = delta / 1000;

	int hours = seconds / 3600;
	seconds -= hours * 3600;

	int minutes = seconds / 60;
	seconds -= minutes * 60;

	StringBuffer buffer;

	if (hours > 0)
		buffer << hours << "h ";

	if (minutes > 0)
		buffer << minutes << "m ";

	if (seconds > 0)
		buffer << seconds << "s";

	String retString = buffer.toString();
	lua_pushstring(L, retString.toCharArray());
	return 1;
}

// sendCommand : call command as creature with arguments and target
//  use: CreatureObject(pCreature):sendCommand("commandString", "arguments", pTarget)
//	note: Use LuaCreatureObject() is some occasions to circumvent if pointer is incorrect.
//  returns: nothing
int LuaCreatureObject::sendCommand(lua_State* L) {
	CreatureObject* target = (CreatureObject*) lua_touserdata(L, -1);
	UnicodeString args = lua_tostring(L, -2);
	String cmd = lua_tostring(L, -3);

	Locker locker(realObject);

	realObject->sendCommand(cmd, args, target->getObjectID());

	return 0;
}

// End Mindsoft Added

// getPetControlDevice :  get the pet control device from a creature - Tyclo
//  use: CreatureObject(pCreature):getPetControlDevice()
//  returns: pointer to a control device
int LuaCreatureObject::getPetControlDevice(lua_State* L) {
	if (realObject->isPet() || realObject->isVehicleObject()) {
		ManagedReference<PetControlDevice*> pcd = realObject->getControlDevice().get().castTo<PetControlDevice*>();
		if (pcd != NULL) {
			lua_pushlightuserdata(L, pcd);
		} else {
			lua_pushnil(L);
			return 1;
		}
	} else {
		lua_pushnil(L);
		return 1;
	}

	return 1;
}
