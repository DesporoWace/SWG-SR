object_draft_schematic_furniture_furniture_table_coffee_modern_02 = object_draft_schematic_furniture_shared_furniture_table_coffee_modern_02:new {
	templateType = DRAFTSCHEMATIC,

	craftingToolTab = 512, -- (See DraftSchematicObjectTemplate.h)
	complexity = 27,
	size = 1,
	factoryCrateSize = 0,

	xpType = "crafting_structure_general",
	xp = 350,

	assemblySkill = "structure_assembly",
	experimentingSkill = "structure_experimentation",
	customizationSkill = "structure_assembly",

	customizationOptions = {2},
	customizationStringNames = {"/private/index_color_1"},
	customizationDefaults = {11},

	ingredientTemplateNames = {"craft_furniture_ingredients_n", "craft_furniture_ingredients_n"},
	ingredientTitleNames = {"frame", "table_top"},
	ingredientSlotType = {0, 0},
	resourceTypes = {"metal_nonferrous", "copper"},
	resourceQuantities = {100, 75},
	contribution = {100, 100},

	targetTemplate = "object/tangible/furniture/modern/coffee_table_modern_style_02.iff",

	additionalTemplates = {}
}
ObjectTemplates:addTemplate(object_draft_schematic_furniture_furniture_table_coffee_modern_02, "object/draft_schematic/furniture/furniture_table_coffee_modern_02.iff")
