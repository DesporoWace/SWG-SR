krayt_dragon_ancient = Creature:new {
	objectName = "@mob/creature_names:krayt_dragon_ancient",
	socialGroup = "krayt",
	faction = "",
	level = 336,
	chanceHit = 30.0,
	damageMin = 2270,
	damageMax = 4250,
	baseXp = 28549,
	baseHAM = 460000,
	baseHAMmax = 551000,
	armor = 3,
	resists = {195,195,195,195,165,195,195,195,105},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_bristley",
	hideAmount = 950,
	boneType = "bone_mammal",
	boneAmount = 905,
	milk = 0,
	tamingChance = 0,
	ferocity = 30,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,
	scale = 1.5,

	templates = {"object/mobile/krayt_dragon_hue.iff"},
	hues = { 16, 17, 18, 19, 20, 21, 22, 23 },
	lootGroups = {
		{
	    groups = {
				{group = "krayt_tissue_common", chance = 2500000}, --25%
				{group = "krayt_tissue_uncommon", chance = 1500000}, --15%
				{group = "krayt_tissue_rare", chance = 1500000}, --15%
				{group = "krayt_pearls", chance = 1500000}, --15%
				{group = "krayt_pearls_lesser", chance = 500000}, --5%
				{group = "armor_attachments", chance = 500000}, --5%
				{group = "clothing_attachments", chance = 500000}, --5%
				{group = "improved_skill_buffs", chance = 1500000}, --15%
			},
			lootChance = 10000000,
		},
		{
	    groups = {
				{group = "krayt_pearls", chance = 1500000}, --15%
				{group = "krayt_pearls_lesser", chance = 2000000}, --20%
				{group = "armor_all", chance = 1250000}, --12.5%
				{group = "weapons_all", chance = 1500000}, --15%
				{group = "wearables_all", chance = 500000}, --5%
				{group = "advanced_skill_buffs", chance = 2250000}, --22.5%
				{group = "experimental_skill_buffs", chance = 1000000}, --10%
			},
			lootChance = 4500000,
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareacombo","stateAccuracyBonus=100"},
		{"creatureareaknockdown","stateAccuracyBonus=100"}
	}
}

CreatureTemplates:addCreatureTemplate(krayt_dragon_ancient, "krayt_dragon_ancient")
