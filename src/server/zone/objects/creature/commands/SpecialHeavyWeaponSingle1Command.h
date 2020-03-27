/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SPECIALHEAVYWEAPONSINGLE1COMMAND_H_
#define SPECIALHEAVYWEAPONSINGLE1COMMAND_H_

#include "CombatQueueCommand.h"

class SpecialHeavyWeaponSingle1Command : public CombatQueueCommand {
public:

	SpecialHeavyWeaponSingle1Command(const String& name, ZoneProcessServer* server)
		: CombatQueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<WeaponObject*> weapon = creature->getWeapon();

		if (weapon->isFlameThrower() || weapon->isHeavyAcidRifle())
			return doCombatAction(creature, target);
		
		return INVALIDWEAPON;
	}
	
	String getAnimation(TangibleObject* attacker, TangibleObject* defender, WeaponObject* weapon, uint8 hitLocation, int damage) const {
		String anim = "";
		if (weapon->isFlameThrower()) {
			anim = "fire_flame_thrower_single_1";
		} else if (weapon->isHeavyAcidRifle()) {
			anim = "fire_acid_rifle_single_1";
		} else {
			warning("Invalid weapon in specialyHeavyWeaponCone1 getAnimation");
			return "";
		}
		
		return anim + getIntensity(weapon->getMaxDamage() / 2.0f, damage);
	}

};

#endif //SPECIALHEAVYWEAPONSINGLE1COMMAND_H_
