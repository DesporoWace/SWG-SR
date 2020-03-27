crackdown_elite_dark_trooper_comd = Creature:new {
	objectName = "@mob/creature_names:crackdown_elite_dark_trooper",
	customName = "Commander",
	randomNameType = NAME_DARKTROOPER,
	socialGroup = "imperial",
	faction = "imperial",
	level = 1,
	chanceHit = 0.64,
	damageMin = 465,
	damageMax = 640,
	baseXp = 45,
	baseHAM = 11000,
	baseHAMmax = 14000,
	armor = 0,
	resists = {0,0,0,30,-1,30,-1,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,
	scale = 1.5,

	templates = {"object/mobile/dark_trooper.iff"},
	lootGroups = {
		{
			groups = {
				{group = "color_crystals", chance = 100000},
				{group = "junk", chance = 7650000},
				{group = "rifles", chance = 550000},
				{group = "pistols", chance = 550000},
                {group = "melee_weapons", chance = 550000},
                {group = "carbines", chance = 550000},
				{group = "clothing_attachments", chance = 25000},
				{group = "armor_attachments", chance = 25000}
			},
			lootChance = 2800000
		}
	},
	weapons = {"stormtrooper_weapons"},
	conversationTemplate = "mandalore_sp",
	attacks = merge(riflemanmaster,carbineermaster)
}

CreatureTemplates:addCreatureTemplate(crackdown_elite_dark_trooper_comd, "crackdown_elite_dark_trooper_comd")
