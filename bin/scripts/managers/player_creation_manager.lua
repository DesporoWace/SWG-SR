--Should all created players start with God Mode? 1 = yes, 0 = no
freeGodMode = 0;
--How many cash credits new characters start with after creating a character (changed during test phase, normal value is 100)
startingCash = 10000
--startingCash = 100000
--How many bank credits new characters start with after creating a character (changed during test phase, normal value is 1000)
startingBank = 100000
--startingBank = 100000
--How many skill points a new characters start with
skillPoints = 250

professions = {
	"combat_brawler",
	"combat_marksman",
	"crafting_artisan",
	"jedi",
	"outdoors_scout",
	"science_medic",
	"social_entertainer"
}

marksmanPistol = "object/weapon/ranged/pistol/pistol_cdef.iff"
marksmanRifle = "object/weapon/ranged/rifle/rifle_cdef.iff"
marksmanCarbine = "object/weapon/ranged/carbine/carbine_cdef.iff"

brawlerOneHander = "object/weapon/melee/knife/knife_stone.iff"
brawlerTwoHander = "object/weapon/melee/axe/axe_heavy_duty.iff"
brawlerPolearm = "object/weapon/melee/polearm/lance_staff_wood_s1.iff"
survivalKnife = "object/weapon/melee/knife/knife_survival.iff"

genericTool = "object/tangible/crafting/station/generic_tool.iff"
mineralTool = "object/tangible/survey_tool/survey_tool_mineral.iff"
chemicalTool = "object/tangible/survey_tool/survey_tool_liquid.iff"

foodTool = "object/tangible/crafting/station/food_tool.iff"

slitherhorn = "object/tangible/instrument/slitherhorn.iff"

marojMelon = "object/tangible/food/foraged/foraged_fruit_s1.iff"

swoopSpeeder = "object/tangible/deed/vehicle_deed/speederbike_swoop_deed.iff"

barcSpeeder = "object/tangible/deed/vehicle_deed/barc_speeder_deed.iff"


armor_npe_zam_wesell_boots = "object/tangible/wearables/armor/npe/zam/armor_zam_wesell_npe_boots.iff"
armor_npe_zam_wesell_chest_plate = "object/tangible/wearables/armor/npe/zam/armor_zam_wesell_npe_chest_plate.iff"
armor_npe_zam_wesell_gloves = "object/tangible/wearables/armor/npe/zam/armor_zam_wesell_npe_gloves.iff"
armor_npe_zam_wesell_helmet = "object/tangible/wearables/armor/npe/zam/armor_zam_wesell_npe_helmet.iff"
armor_npe_zam_wesell_pants = "object/tangible/wearables/armor/npe/zam/armor_zam_wesell_npe_pants.iff"

armor_npe_ubese_boots = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_boots.iff"
armor_npe_ubese_jacket = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_jacket.iff"
armor_npe_ubese_bracer_l = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_bracer_l.iff"
armor_npe_ubese_bracer_r = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_bracer_r.iff"
armor_npe_ubese_gloves = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_gloves.iff"
armor_npe_ubese_helmet = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_helmet.iff"
armor_npe_ubese_pants = "object/tangible/wearables/armor/npe/ubese/armor_ubese_npe_pants.iff"

armor_npe_padded_bicep_l = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_bicep_l.iff"
armor_npe_padded_bicep_r = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_bicep_r.iff"
armor_npe_padded_boots = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_boots.iff"
armor_npe_padded_chest_plate = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_chest_plate.iff"
armor_npe_padded_bracer_l = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_bracer_l.iff"
armor_npe_padded_bracer_r = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_bracer_r.iff"
armor_npe_padded_gloves = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_gloves.iff"
armor_npe_padded_helmet = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_helmet.iff"
armor_npe_padded_leggings = "object/tangible/wearables/armor/npe/padded/armor_padded_npe_leggings.iff"

professionSpecificItems = {
	combat_brawler = { brawlerOneHander, brawlerTwoHander, brawlerPolearm, armor_npe_ubese_boots, armor_npe_ubese_jacket, armor_npe_ubese_bracer_l, armor_npe_ubese_bracer_r, armor_npe_ubese_gloves, armor_npe_ubese_helmet, armor_npe_ubese_pants },
	combat_marksman = { marksmanCarbine, marksmanRifle, armor_npe_zam_wesell_boots, armor_npe_zam_wesell_chest_plate, armor_npe_zam_wesell_gloves, armor_npe_zam_wesell_helmet, armor_npe_zam_wesell_pants },
	crafting_artisan = { genericTool, mineralTool, chemicalTool, armor_npe_padded_bicep_l, armor_npe_padded_bicep_r, armor_npe_padded_boots, armor_npe_padded_chest_plate, armor_npe_padded_bracer_l, armor_npe_padded_bracer_r, armor_npe_padded_gloves, armor_npe_padded_helmet, armor_npe_padded_leggings },
	jedi = { },
	outdoors_scout = { genericTool, armor_npe_ubese_boots, armor_npe_ubese_jacket, armor_npe_ubese_bracer_l, armor_npe_ubese_bracer_r, armor_npe_ubese_gloves, armor_npe_ubese_helmet, armor_npe_ubese_pants },
	science_medic = { foodTool, armor_npe_padded_bicep_l, armor_npe_padded_bicep_r, armor_npe_padded_boots, armor_npe_padded_chest_plate, armor_npe_padded_bracer_l, armor_npe_padded_bracer_r, armor_npe_padded_gloves, armor_npe_padded_helmet, armor_npe_padded_leggings },
	social_entertainer = { slitherhorn, armor_npe_zam_wesell_boots, armor_npe_zam_wesell_chest_plate, armor_npe_zam_wesell_gloves, armor_npe_zam_wesell_helmet, armor_npe_zam_wesell_pants }
}

--[[ Old
professionSpecificItems = {
	combat_brawler = { brawlerOneHander, brawlerTwoHander, brawlerPolearm },
	combat_marksman = { marksmanPistol, marksmanCarbine, marksmanRifle },
	crafting_artisan = { genericTool, mineralTool, chemicalTool },
	jedi = { },
	outdoors_scout = { genericTool },
	science_medic = { foodTool },
	social_entertainer = { slitherhorn }
}
]]

commonStartingItems = { marojMelon, survivalKnife, marksmanPistol, swoopSpeeder }
