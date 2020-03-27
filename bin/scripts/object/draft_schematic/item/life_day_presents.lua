object_draft_schematic_item_life_day_presents = object_draft_schematic_item_shared_life_day_presents:new {
	templateType = DRAFTSCHEMATIC,

	customObjectName = "Crafted Life Day Presents",

	craftingToolTab = 524288, -- (See DraftSchematicObjectTemplate.h)
	complexity = 15,
	size = 2,
	factoryCrateSize = 0,

  xpType = "crafting_general",
	xp = 200,

	assemblySkill = "general_assembly",
	experimentingSkill = "general_experimentation",
	customizationSkill = "general_customization",

	customizationOptions = {},
	customizationStringNames = {},
	customizationDefaults = {},

	ingredientTemplateNames = {"craft_furniture_ingredients_n", "craft_furniture_ingredients_n", "craft_furniture_ingredients_n"},
	ingredientTitleNames = {"wrapping", "repair_presents", "broken_presents"},
	ingredientSlotType = {0, 0, 2},
	resourceTypes = {"petrochem_inert_polymer", "metal", "object/tangible/event_perk/shared_life_day_broken_presents.iff"},
	resourceQuantities = {100, 150, 5},
	contribution = {100, 100, 100},


	targetTemplate = "object/tangible/event_perk/life_day_crafted_presents.iff",

	additionalTemplates = {
	}
}

ObjectTemplates:addTemplate(object_draft_schematic_item_life_day_presents, "object/draft_schematic/item/life_day_presents.iff")
