ForceChokeCommand = {
	name = "forcechoke",

	minDamage = 375,
	maxDamage = 420,
	speed = 4.0,
	forceCost = 340,
	visMod = 25,
	forceAttack = true,
	accuracyBonus = 95,

	animation = "force_choke_1_particle_level_1",
	animType = GENERATE_INTENSITY,

	combatSpam = "forcechoke",

	poolsToDamage = HEALTH_ATTRIBUTE,
	damageType = LIGHTSABER_DAMAGE,

	range = 32,

	dotEffects = {
		DotEffect(
			FORCECHOKE,
			{ "", "" },
			HEALTH,
			true,
			-1,
			100,
			350,
			30,
			100,
			1
		)
	},

	frsLightMinDamageModifier = 1,
	frsLightMaxDamageModifier = 4,
	frsDarkMinDamageModifier = 1,
	frsDarkMaxDamageModifier = 4,
}

AddCommand(ForceChokeCommand)
