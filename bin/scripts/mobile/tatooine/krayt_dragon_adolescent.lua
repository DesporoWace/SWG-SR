krayt_dragon_adolescent = Creature:new {
	objectName = "@mob/creature_names:krayt_dragon_adolescent",
	socialGroup = "krayt",
	faction = "",
	level = 300,
	chanceHit = 30,
	damageMin = 2040,
	damageMax = 3825,
	baseXp = 25694,
	baseHAM = 369000,
	baseHAMmax = 451000,
	armor = 3,
	resists = {185,185,185,185,150,185,185,185,105},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_bristley",
	hideAmount = 750,
	boneType = "bone_mammal",
	boneAmount = 675,
	milk = 0,
	tamingChance = 0,
	ferocity = 30,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/krayt_dragon_hue.iff"},
	hues = { 16, 17, 18, 19, 20, 21, 22, 23 },
	scale = 0.7;
	lootGroups = {
		{
	    groups = {
				{group = "krayt_tissue_common", chance = 2500000},
				{group = "krayt_tissue_uncommon", chance = 1500000},
				{group = "krayt_tissue_rare", chance = 750000},
				{group = "krayt_pearls", chance = 1500000},
				{group = "krayt_pearls_lesser", chance = 2000000},
				{group = "armor_attachments", chance = 875000},
				{group = "clothing_attachments", chance = 875000}
			},
			lootChance = 8000000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareacombo",""},
		{"creatureareaknockdown",""}
	}
}

CreatureTemplates:addCreatureTemplate(krayt_dragon_adolescent, "krayt_dragon_adolescent")
