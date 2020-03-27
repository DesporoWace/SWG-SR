gungan_stun_lance = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "",
	directObjectTemplate = "object/weapon/melee/polearm/polearm_gungan_stun_lance.iff",
	craftingValues = {
		{"mindamage",11,20,0},
		{"maxdamage",60,111,0},
		{"attackspeed",4.2,2.9,1},
		{"woundchance",6,12,0},
		{"hitpoints",750,1500,0},
		{"zerorangemod",-2,8,0},
		{"maxrangemod",-2,8,0},
		{"midrange",3,3,0},
		{"midrangemod",-2,8,0},
		{"maxrange",4,4,0},		
		{"attackhealthcost",12,2,0},
		{"attackactioncost",25,11,0},
		{"attackmindcost",12,2,0},
	},
	customizationStringNames = {},
	customizationValues = {},
	-- randomDotChance: The chance of this weapon object dropping with a random dot on it. Higher number means less chance. Set to 0 to always have a random dot.
	randomDotChance = 750,
	junkDealerTypeNeeded = JUNKWEAPONS,
	junkMinValue = 25,
	junkMaxValue = 45
}

addLootItemTemplate("gungan_stun_lance", gungan_stun_lance)
