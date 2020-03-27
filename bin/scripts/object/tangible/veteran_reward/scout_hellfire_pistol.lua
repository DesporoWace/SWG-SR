object_draft_schematic_weapon_pistols_scout_hellfire_pistol = object_draft_schematic_weapon_pistols_shared_scout_hellfire_pistol:new {
   templateType = DRAFTSCHEMATIC,

   customObjectName = "Scout Hellfire Pistol",

   craftingToolTab = 1, -- (See DraftSchematicObjectTemplate.h)
   complexity = 20, 
   size = 3, 
	 factoryCrateSize = 0,

   xpType = "crafting_weapons_general", 
   xp = 60, 

   assemblySkill = "weapon_assembly", 
   experimentingSkill = "weapon_experimentation", 
   customizationSkill = "weapon_customization", 

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"frame_assembly", "receiver_assembly", "grip_assembly", "powerhandler", "barrel", "scope"},
   ingredientSlotType = {0, 0, 0, 1, 1, 3},
   resourceTypes = {"metal_nonferrous", "metal", "metal", "object/tangible/component/weapon/shared_blaster_power_handler.iff", "object/tangible/component/weapon/shared_blaster_pistol_barrel.iff", "object/tangible/component/weapon/shared_scope_weapon.iff"},
   resourceQuantities = {18, 8, 5, 1, 1, 1},
   contribution = {100, 100, 100, 100, 100, 100},
   ingredientAppearance = {"", "", "", "", "", ""},

   targetTemplate = "object/weapon/ranged/pistol/scout_hellfire.iff",

   additionalTemplates = {
             }

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_pistols_scout_hellfire_pistol, "object/draft_schematic/weapon/pistols/scout_hellfire_pistol.iff")
