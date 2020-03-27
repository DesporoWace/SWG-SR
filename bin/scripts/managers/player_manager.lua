--Copyright (C) 2007 <SWGEmu>

--This File is part of Core3.

--This program is free software; you can redistribute
--it and/or modify it under the terms of the GNU Lesser
--General Public License as published by the Free Software
--Foundation; either version 2 of the License,
--or (at your option) any later version.

--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
--See the GNU Lesser General Public License for
--more details.

--You should have received a copy of the GNU Lesser General
--Public License along with this program; if not, write to
--the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--Linking Engine3 statically or dynamically with other modules
--is making a combined work based on Engine3.
--Thus, the terms and conditions of the GNU Lesser General Public License
--cover the whole combination.

--In addition, as a special exception, the copyright holders of Engine3
--give you permission to combine Engine3 program with free software
--programs or libraries that are released under the GNU LGPL and with
--code included in the standard release of Core3 under the GNU LGPL
--license (or modified versions of such code, with unchanged license).
--You may copy and distribute such a system following the terms of the
--GNU LGPL for Engine3 and the licenses of the other code concerned,
--provided that you include the source code of that other code when
--and as the GNU LGPL requires distribution of source code.

--Note that people who make modified versions of Engine3 are not obligated
--to grant this special exception for their modified versions;
--it is their choice whether to do so. The GNU Lesser General Public License
--gives permission to release a modified version without this exception;
--this exception also makes it possible to release a modified version
--which carries forward this exception.

-----------------------------------
--GENERAL PLAYER SETTINGS
-----------------------------------
onlineCharactersPerAccount = 3 --How many characters are allowed online from a single account.
allowSameAccountPvpRatingCredit = 0

--Blue frog / GM buff values
performanceBuff = 2000
medicalBuff = 2500
performanceDuration = 3600 -- in seconds, 3600 default
medicalDuration = 3600 -- in seconds, 7200 default

--Sets the experience multiplier while grouped
groupExpMultiplier = 1.0

--Sets a global experience multiplier
globalExpMultiplier = 5.0

--Sets the base number of control devices of each type that a player can have in their datapad at once
--For creature pets, A Creature Handler will have the base number + their stored pets skill mod as limit
baseStoredCreaturePets = 5
baseStoredFactionPets = 5
baseStoredDroids = 5
baseStoredVehicles = 5
baseStoredShips = 3
baseStoredFamiliarPets = 9

-----------------------------------
--VETERAN REWARDS CONFIG
-----------------------------------
veteranRewardMilestones = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 150, 180, 210, 240, 270, 300, 330, 360} --days, must be sorted low to high

veteranRewardAdditionalMilestones = 30 --frequency, in days, of additional milestones (beyond the established ones)

veteranRewards = {

	{templateFile = "object/tangible/veteran_reward/data_terminal_s1.iff", milestone=1, oneTime=false, description="Holonet Tracking Console"},
	{templateFile = "object/tangible/veteran_reward/data_terminal_s2.iff", milestone=1, oneTime=false, description="Sample Readout Data Console"},
	{templateFile = "object/tangible/veteran_reward/data_terminal_s3.iff", milestone=1, oneTime=false, description="Holonet Terminal"},
	{templateFile = "object/tangible/veteran_reward/data_terminal_s4.iff", milestone=1, oneTime=false, description="Control Array"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_protocol_droid_toy.iff", milestone=1, oneTime=false, description="Mini Protocol Droid"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_r2_toy.iff", milestone=1, oneTime=false, description="Mini R2D2"},

	-- 10 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_couch_falcon_corner_s01.iff", milestone=10, oneTime=false, description="YT1300 Couch"},
	{templateFile = "object/tangible/veteran_reward/frn_couch_falcon_section_s01.iff", milestone=10, oneTime=false, description="YT1300 Chair"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_tie_fighter_toy.iff", milestone=10, oneTime=false, description="Mini Tie Fighter"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_x_wing_toy.iff", milestone=10, oneTime=false, description="Mini X-Wing"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_darth_vader_toy.iff", milestone=10, oneTime=false, description="Mini Darth Vader"},

	-- 20 Day Rewards
	{templateFile = "object/tangible/deed/vehicle_deed/speederbike_flash_deed.iff", milestone=20, oneTime=true},
	{templateFile = "object/tangible/wearables/goggles/goggles_s01.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 1"},
	{templateFile = "object/tangible/wearables/goggles/goggles_s02.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 2"},
	{templateFile = "object/tangible/wearables/goggles/goggles_s03.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 3"},
	{templateFile = "object/tangible/wearables/goggles/goggles_s04.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 4"},
	{templateFile = "object/tangible/wearables/goggles/goggles_s05.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 5"},
	{templateFile = "object/tangible/wearables/goggles/goggles_s06.iff", milestone=20, oneTime=false, description="Special Edition Goggles Style 6"},

	-- 30 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_tech_console_sectional_a.iff", milestone=30, oneTime=false, description="Holonet Databank "},
	{templateFile = "object/tangible/veteran_reward/frn_tech_console_sectional_b.iff", milestone=30, oneTime=false, description="System Readout Terminal "},
	{templateFile = "object/tangible/veteran_reward/frn_tech_console_sectional_c.iff", milestone=30, oneTime=false, description="Database Input Terminal "},
	{templateFile = "object/tangible/veteran_reward/frn_tech_console_sectional_d.iff", milestone=30, oneTime=false, description="Main Systems Databank "},
	{templateFile = "object/tangible/veteran_reward/frn_vet_jabba_toy.iff", milestone=30, oneTime=false, description="Mini Jabba"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_stormtrooper_toy.iff", milestone=30, oneTime=false, description="Mini Stormtrooper"},

	-- 40 Day Rewards
	{templateFile = "object/tangible/camp/camp_spit_s2.iff", milestone=40, oneTime=false, description="Camp Center (Small)"},
	{templateFile = "object/tangible/camp/camp_spit_s3.iff", milestone=40, oneTime=false, description="Camp Center (Large)"},
	{templateFile = "object/tangible/furniture/tatooine/frn_tato_vase_style_01.iff", milestone=40, oneTime=false, description="Gold Ornamental Vase (Short)"},
	{templateFile = "object/tangible/furniture/tatooine/frn_tato_vase_style_02.iff", milestone=40, oneTime=false, description="Gold Ornamental Vase (Tall)"},
	{templateFile = "object/tangible/furniture/decorative/foodcart.iff", milestone=40, oneTime=false, description="Foodcart"},
	{templateFile = "object/tangible/furniture/all/frn_bench_generic.iff", milestone=40, oneTime=false, description="Park Bench"},

	-- 50 Day Rewards
	{templateFile = "object/tangible/furniture/decorative/professor_desk.iff", milestone=50, oneTime=false, description="Professor Desk"},
	{templateFile = "object/tangible/furniture/decorative/diagnostic_screen.iff", milestone=50, oneTime=false, description="Small Diagnostic Screen"},
	{templateFile = "object/tangible/furniture/all/frn_all_plant_potted_lg_s2.iff", milestone=50, oneTime=false, description="Large Potted Plant (Cactus)"},
	{templateFile = "object/tangible/furniture/all/frn_all_plant_potted_lg_s3.iff", milestone=50, oneTime=false, description="Large Potted Plant (3-Tiered)"},
	{templateFile = "object/tangible/furniture/all/frn_all_plant_potted_lg_s4.iff", milestone=50, oneTime=false, description="Large Potted Plant (Droopy)"},

	-- 60 Day Rewards
	{templateFile = "object/tangible/furniture/modern/bar_counter_s1.iff", milestone=60, oneTime=false, description="Bar Countertop (Straight, Short)"},
	{templateFile = "object/tangible/furniture/modern/bar_piece_curve_s1.iff", milestone=60, oneTime=false, description="Bar Countertop (Curved  Corner, Short)"},
	{templateFile = "object/tangible/furniture/modern/bar_piece_curve_s2.iff", milestone=60, oneTime=false, description="Bar Countertop (Curved Corner, Long)"},
	{templateFile = "object/tangible/furniture/modern/bar_piece_straight_s1.iff", milestone=60, oneTime=false, description="Bar Countertop (Square Corner)"},
	{templateFile = "object/tangible/furniture/modern/bar_piece_straight_s2.iff", milestone=60, oneTime=false, description="Bar Countertop (Straight, Long)"},

	-- 70 Day Rewards
	{templateFile = "object/tangible/furniture/all/frn_all_table_s01.iff", milestone=70, oneTime=false, description="Round Cantina Table (Solid Sides)"},
	{templateFile = "object/tangible/furniture/all/frn_all_table_s02.iff", milestone=70, oneTime=false, description="Round Cantina Table (Angled Sides)"},
	{templateFile = "object/tangible/furniture/all/frn_all_table_s03.iff", milestone=70, oneTime=false, description="Round Cantina Table (Pedestal)"},
	{templateFile = "object/tangible/furniture/tatooine/frn_tatt_chair_cantina_seat_2.iff", milestone=70, oneTime=false, description="Large Cantina Sofa"},
	{templateFile = "object/tangible/furniture/tatooine/frn_tato_cafe_parasol.iff", milestone=70, oneTime=false, description="Cafe Parasol"},

	-- 80 Day Rewards
	{templateFile = "object/tangible/furniture/modern/rug_oval_m_s02.iff", milestone=80, oneTime=false, description="Medium Tan Oval Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_oval_sml_s01.iff", milestone=80, oneTime=false, description="Small Purple Oval Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_rect_m_s01.iff", milestone=80, oneTime=false, description="Medium Animal Print Rectangular Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_rect_sml_s01.iff", milestone=80, oneTime=false, description="Small Patchwork Rectangular Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_rnd_m_s01.iff", milestone=80, oneTime=false, description="Medium Tan Round Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_rnd_sml_s01.iff", milestone=80, oneTime=false, description="Small Purple Round Rug"},

	-- 90 Day Rewards
	{templateFile = "object/tangible/loot/misc/loot_skull_bith.iff", milestone=90, oneTime=false, description="A Bith Skull"},
	{templateFile = "object/tangible/loot/misc/loot_skull_human.iff", milestone=90, oneTime=false, description="A Human Skull"},
	{templateFile = "object/tangible/loot/misc/loot_skull_ithorian.iff", milestone=90, oneTime=false, description="A Ithorian Skull"},
	{templateFile = "object/tangible/furniture/all/frn_all_potted_plants_sml_s03.iff", milestone=90, oneTime=false, description="Small Potted Plant (Sago Palm)"},
  	{templateFile = "object/tangible/furniture/all/frn_all_potted_plants_sml_s04.iff", milestone=90, oneTime=false, description="Small Potted Plant (Varigated Leaves)"},
  	{templateFile = "object/tangible/furniture/all/frn_all_potted_plants_sml_s05.iff", milestone=90, oneTime=false, description="Small Potted Plant (Small Eucalyptus)"},

	-- 100 Day Rewards
	{templateFile = "object/tangible/furniture/modern/rug_rect_lg_s01.iff", milestone=10, oneTime=false, description="Large Rectangular Rug (Wavy Lines)"},
	{templateFile = "object/tangible/furniture/modern/rug_rect_lg_s02.iff", milestone=100, oneTime=false, description="Large Rectangular Rug (Orange Abstract)"},
	{templateFile = "object/tangible/furniture/modern/rug_oval_lg_s01.iff", milestone=100, oneTime=false, description="Large Brown Oval Rug"},
	{templateFile = "object/tangible/furniture/modern/rug_rnd_lg_s01.iff", milestone=100, oneTime=false, description="Large Tan Round Rug"},
	{templateFile = "object/tangible/furniture/all/frn_all_desk_map_table.iff", milestone=100, oneTime=false, description="Round Data Terminal"},
	{templateFile = "object/tangible/tcg/series1/decorative_computer_console_01.iff", milestone=100, oneTime=false, description="Large Computer Console"},

	-- 110 Day Rewards
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_01.iff", milestone=110, oneTime=false, description="Painting: Cast Wing in Flight"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_02.iff", milestone=110, oneTime=false, description="Painting: Decimator"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_03.iff", milestone=110, oneTime=false, description="Painting: Tatooine Dune Speeder"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_04.iff", milestone=110, oneTime=false, description="Painting: Weapon of War"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_05.iff", milestone=110, oneTime=false, description="Painting: Fighter Study"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_06.iff", milestone=110, oneTime=false, description="Painting: Hutt Greed"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_07.iff", milestone=110, oneTime=false, description="Painting: Smuggler's Run"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_08.iff", milestone=110, oneTime=false, description="Painting: Imperial Oppression (TIE Oppressor)"},
	{templateFile = "object/tangible/veteran_reward/one_year_anniversary/painting_09.iff", milestone=110, oneTime=false, description="Painting: Emperor's Eyes (TIE Sentinel)"},

	-- 120 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_vet_houseplant.iff", milestone=120, oneTime=false, description="Green Exotic Houseplant"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_houseplant_02.iff", milestone=120, oneTime=false, description="Orange Exotic Houseplant"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_display_stand.iff", milestone=120, oneTime=false, description="Figurine Display Stand"},

	-- 150 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_gaping_spider_toy.iff", milestone=150, oneTime=false, description="Mini Gaping Spider"},
	{templateFile = "object/tangible/veteran_reward/frn_mutated_rancor_toy.iff", milestone=150, oneTime=false, description="Mini Mutated Rancor"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_dewback_toy.iff", milestone=150, oneTime=false, description="Mini Dewback"},
	{templateFile = "object/tangible/veteran_reward/frn_malkloc_toy.iff", milestone=150, oneTime=false, description="Mini Malkloc"},

	-- 180 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_vet_atat_toy.iff", milestone=180, oneTime=false, description="Mini ATAT"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_tusken_raider_toy.iff", milestone=180, oneTime=false, description="Mini Tusken Raider"},
	{templateFile = "object/tangible/veteran_reward/frn_shear_mite_toy.iff", milestone=180, oneTime=false, description="Mini Shear Mite"},
	{templateFile = "object/tangible/veteran_reward/frn_reptilian_flyer_toy.iff", milestone=180, oneTime=false, description="Mini Reptilian Flyer"},

	-- 210 Day Rewards
	{templateFile = "object/tangible/veteran_reward/frn_vet_rancor_toy.iff", milestone=210, oneTime=false, description="Mini Rancor"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_salacious_crumb_toy.iff", milestone=210, oneTime=false, description="Mini Salacious Crumb"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_boba_toy.iff", milestone=210, oneTime=false, description="Mini Boba Fett"},

	-- 240 Day Rewards
	{templateFile = "object/tangible/tcg/series4/decorative_han_solo_statuette.iff", milestone=240, oneTime=false, description="Han Solo Statuette"},
	{templateFile = "object/tangible/tcg/series4/decorative_chewbacca_statuette.iff", milestone=240, oneTime=false, description="Chewbacca Statuette"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_fireplace.iff", milestone=240, oneTime=false, description="Round Fireplace"},

	-- 300 Day Rewards
	{templateFile = "object/tangible/tcg/series5/decorative_skywalker_statuette.iff", milestone=300, oneTime=false, description="Luke Skywalker Statuette"},
	{templateFile = "object/tangible/tcg/series1/decorative_statuette_princess_leia.iff", milestone=300, oneTime=false, description="Princess Leia Statuette"},
	{templateFile = "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_03.iff", milestone=300, oneTime=false, description="Large Brown Rectangle Rug"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_jedi_temple.iff", milestone=300, oneTime=false, description="Jedi Temple"},

	-- 330 Day Rewards
	{templateFile = "object/tangible/tcg/series1/decorative_display_case_02.iff", milestone=330, oneTime=false, description="Short Glass Display Case"},
	{templateFile = "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_01.iff", milestone=330, oneTime=false, description="Large Red rectangle Rug"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_emp_chair.iff", milestone=330, oneTime=false, description="Emperor's Chair"},
	{templateFile = "object/tangible/veteran_reward/frn_vet_jedi_temple_burning.iff", milestone=330, oneTime=false, description="Burning Jedi Temple"},

	-- 360 Day Rewards
	{templateFile = "object/tangible/tcg/series1/decorative_computer_console_02.iff", milestone=360, oneTime=false, description="Desk with Computer"},
	{templateFile = "object/tangible/tcg/series1/decorative_display_case_01.iff", milestone=360, oneTime=false, description="Tall Glass Display Case"},
	{templateFile = "object/tangible/furniture/all/frn_all_rug_rectangle_large_style_05.iff", milestone=360, oneTime=false, description="Large Green Square Rug"},

}

-- { "stringId", "songFilePath" }
jukeboxSongs = {
	{ "@event_perk_jukebox_songs:emperors_theme", "sound/music_emperor_theme_loop.snd" },
	{ "@event_perk_jukebox_songs:chamber_music", "sound/music_starport_a_loop.snd" },
	{ "@event_perk_jukebox_songs:hard_rock", "sound/music_starport_b_loop.snd" },
	{ "@event_perk_jukebox_songs:theed_palace", "sound/music_theed_palace_loop.snd" },
	{ "@event_perk_jukebox_songs:medley", "sound/music_autorun_loop.snd" },
	{ "@event_perk_jukebox_songs:otoh_gunga", "sound/music_otoh_gunga_loop.snd" },
	{ "@event_perk_jukebox_songs:star_wars_intro", "sound/music_intro_loop.snd" },
	{ "@event_perk_jukebox_songs:celeb_phantom_menace", "sound/music_celebration_a_loop.snd" },
	{ "@event_perk_jukebox_songs:celeb_rotj", "sound/music_celebration_b_loop.snd" },
	{ "@event_perk_jukebox_songs:pod_race", "sound/music_combat_bfield_loop.snd" },
	{ "@event_perk_jukebox_songs:figrin_dan_1", "sound/music_figrin_dan_1_loop.snd" },
	{ "@event_perk_jukebox_songs:figrin_dan_2", "sound/music_figrin_dan_2_loop.snd" },
	{ "@event_perk_jukebox_songs:soothing_corellia", "sound/music_id_tent_corellia_loop.snd" },
	{ "@event_perk_jukebox_songs:soothing_naboo", "sound/music_id_tent_naboo_loop.snd" },
	{ "@event_perk_jukebox_songs:soothing_tatooine", "sound/music_id_tent_tatooine_loop.snd" },
	{ "@event_perk_jukebox_songs:max_rebo_1", "sound/music_max_rebo_1_loop.snd" },
	{ "@event_perk_jukebox_songs:max_rebo_2", "sound/music_max_rebo_2_loop.snd" },
	{ "@event_perk_jukebox_songs:romance_1", "sound/music_romance_a_loop.snd" },
	{ "@event_perk_jukebox_songs:romance_2", "sound/music_romance_b_loop.snd" },
	{ "@event_perk_jukebox_songs:romance_3", "sound/music_romance_c_loop.snd" },
	{ "@event_perk_jukebox_songs:satisfaction_1", "sound/music_satisfaction_a_loop.snd" },
	{ "@event_perk_jukebox_songs:satisfaction_2", "sound/music_satisfaction_b_loop.snd" },
	{ "@event_perk_jukebox_songs:exar_theme", "sound/music_exar_theme_loop.snd" },
	{ "@event_perk_jukebox_songs:exploration", "sound/music_explore_a_loop.snd" },
	{ "@event_perk_jukebox_songs:humor_1", "sound/music_humor_a_loop.snd" },
	{ "@event_perk_jukebox_songs:humor_2", "sound/music_humor_b_loop.snd" },
	{ "@event_perk_jukebox_songs:leia_theme", "sound/music_leia_theme_loop.snd" },
	{ "@event_perk_jukebox_songs:evil_ambiance", "sound/music_underground_loop.snd" },
	{ "@event_perk_jukebox_songs:eerie_ambiance", "sound/music_underwater_loop.snd" },
	{ "@event_perk_jukebox_songs:lok_theme", "sound/music_gloom_a_loop.snd" }
}
