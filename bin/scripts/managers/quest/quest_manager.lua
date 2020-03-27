local ObjectManager = require("managers.object.object_manager")

local QuestManager = {}

local QUEST_ACTIVE = 1
local QUEST_COMPLETED = 1

-- Activate the quest for the player.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to activate.
function QuestManager.activateQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	if (QuestManager.shouldSendSystemMessage(pCreatureObject, quest)) then
		CreatureObject(pCreatureObject):sendSystemMessage("@quest/quests:quest_journal_updated")
	end

	PlayerObject(pGhost):setActiveQuestsBit(quest, QUEST_ACTIVE)
end

-- Checks if the player has a quest active.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to check if it is active.
function QuestManager.hasActiveQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return false
	end

	return PlayerObject(pGhost):hasActiveQuestBitSet(quest)
end

-- Complete the quest for the player.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to complete.
function QuestManager.completeQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	if (QuestManager.shouldSendSystemMessage(pCreatureObject, quest)) then
		local summary = QuestManager.getJournalSummary(quest)

		if (summary ~= "") then
			CreatureObject(pCreatureObject):sendSystemMessage(summary)
		end

		CreatureObject(pCreatureObject):sendSystemMessage("@quest/quests:task_complete")
	end

	PlayerObject(pGhost):clearActiveQuestsBit(quest)
	PlayerObject(pGhost):setCompletedQuestsBit(quest, QUEST_COMPLETED)
end

-- Un-Complete the quest for the player and set quest active again.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to complete.
function QuestManager.uncompleteQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	PlayerObject(pGhost):clearCompletedQuestsBit(quest)
	PlayerObject(pGhost):setActiveQuestsBit(quest, QUEST_ACTIVE)
end

-- Checks if the player has a quest completed.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to check if it is completed.
function QuestManager.hasCompletedQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return false
	end

	return PlayerObject(pGhost):hasCompletedQuestsBitSet(quest)
end

-- Reset the quest for the player.
-- @param pCreatureObject pointer to the creature object of the player.
-- @param quest the index number for the quest to reset.
function QuestManager.resetQuest(pCreatureObject, quest)
	local pGhost = CreatureObject(pCreatureObject):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	PlayerObject(pGhost):clearActiveQuestsBit(quest)
	PlayerObject(pGhost):clearCompletedQuestsBit(quest)
end

function QuestManager.failQuest(pCreatureObject, quest)
	if (QuestManager.shouldSendSystemMessage(pCreatureObject, quest)) then
		CreatureObject(pCreatureObject):sendSystemMessage("@quest/quests:task_failure")
	end

	QuestManager.resetQuest(pCreatureObject, quest)
end

function QuestManager.shouldSendSystemMessage(pCreatureObject, quest)
	local pQuest = getQuestInfo(quest)

	if (pQuest == nil) then
		return false
	end

	return LuaQuestInfo(pQuest):shouldSendSystemMessage()
end

function QuestManager.getQuestName(questID)
	local pQuest = getQuestInfo(questID)

	if (pQuest == nil) then
		return ""
	end

	return LuaQuestInfo(pQuest):getQuestName()
end

function QuestManager.getJournalSummary(questID)
	local pQuest = getQuestInfo(questID)

	if (pQuest == nil) then
		return ""
	end

	return LuaQuestInfo(pQuest):getJournalSummary()
end

function QuestManager.getCurrentQuestID(pPlayer)
	local id = tonumber(readScreenPlayData(pPlayer, "VillageJediProgression", "CurrentQuestID"))

	if (id == nil) then
		id = -1
	end

	return id
end

function QuestManager.setCurrentQuestID(pPlayer, qid)
	return writeScreenPlayData(pPlayer, "VillageJediProgression", "CurrentQuestID", qid)
end

function QuestManager.getStoredVillageValue(pPlayer, key)
	return readScreenPlayData(pPlayer, "VillageJediProgression", key)
end

function QuestManager.setStoredVillageValue(pPlayer, key, value)
	return writeScreenPlayData(pPlayer, "VillageJediProgression", key, value)
end

QuestManager.quests = {}

QuestManager.quests.TEST_SIMPLE 					= 0
QuestManager.quests.TEST_GOTO_01 					= 1
QuestManager.quests.TEST_GOTO_02 					= 2
QuestManager.quests.TEST_GOTO_03 					= 3
QuestManager.quests.TEST_FIND_01 					= 4
QuestManager.quests.TEST_ENCOUNTER_01 				= 5
QuestManager.quests.TEST_DESTROY_01 				= 6
QuestManager.quests.TEST_ESCORT_01 					= 7
QuestManager.quests.TEST_ESCORT_LOCATION 			= 8
QuestManager.quests.TEST_RANDOM 					= 9
QuestManager.quests.TEST_GIVE 						= 10
QuestManager.quests.SCT1 							= 11
QuestManager.quests.SCT2 							= 12
QuestManager.quests.SCT3 							= 13
QuestManager.quests.SCT4 							= 14
QuestManager.quests.FS_QUESTS_SAD_TASKS 			= 15
QuestManager.quests.FS_QUESTS_SAD_TASK1 			= 16
QuestManager.quests.FS_QUESTS_SAD_RETURN1 			= 17
QuestManager.quests.FS_QUESTS_SAD_TASK2 			= 18
QuestManager.quests.FS_QUESTS_SAD_RETURN2 			= 19
QuestManager.quests.FS_QUESTS_SAD_TASK3 			= 20
QuestManager.quests.FS_QUESTS_SAD_RETURN3 			= 21
QuestManager.quests.FS_QUESTS_SAD_TASK4 			= 22
QuestManager.quests.FS_QUESTS_SAD_RETURN4 			= 23
QuestManager.quests.FS_QUESTS_SAD_TASK5 			= 24
QuestManager.quests.FS_QUESTS_SAD_RETURN5 			= 25
QuestManager.quests.FS_QUESTS_SAD_TASK6 			= 26
QuestManager.quests.FS_QUESTS_SAD_RETURN6 			= 27
QuestManager.quests.FS_QUESTS_SAD_TASK7 			= 28
QuestManager.quests.FS_QUESTS_SAD_RETURN7 			= 29
QuestManager.quests.FS_QUESTS_SAD_TASK8 			= 30
QuestManager.quests.FS_QUESTS_SAD_RETURN8 			= 31
QuestManager.quests.FS_QUESTS_SAD_FINISH 			= 32
QuestManager.quests.FS_QUESTS_SAD2_TASKS 			= 33
QuestManager.quests.FS_QUESTS_SAD2_TASK1			= 34
QuestManager.quests.FS_QUESTS_SAD2_RETURN1 			= 35
QuestManager.quests.FS_QUESTS_SAD2_TASK2 			= 36
QuestManager.quests.FS_QUESTS_SAD2_RETURN2 			= 37
QuestManager.quests.FS_QUESTS_SAD2_TASK3 			= 38
QuestManager.quests.FS_QUESTS_SAD2_RETURN3 			= 39
QuestManager.quests.FS_QUESTS_SAD2_TASK4 			= 40
QuestManager.quests.FS_QUESTS_SAD2_RETURN4 			= 41
QuestManager.quests.FS_QUESTS_SAD2_TASK5 			= 42
QuestManager.quests.FS_QUESTS_SAD2_RETURN5 			= 43
QuestManager.quests.FS_QUESTS_SAD2_TASK6 			= 44
QuestManager.quests.FS_QUESTS_SAD2_RETURN6 			= 45
QuestManager.quests.FS_QUESTS_SAD2_TASK7 			= 46
QuestManager.quests.FS_QUESTS_SAD2_RETURN7 			= 47
QuestManager.quests.FS_QUESTS_SAD2_TASK8 			= 48
QuestManager.quests.FS_QUESTS_SAD2_RETURN8 			= 49
QuestManager.quests.FS_QUESTS_SAD2_FINISH 			= 50
QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_01 		= 51
QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_02 		= 52
QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_03 		= 53
QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_02 	= 54
QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_02 	= 55
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_00 		= 56
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_01 		= 57
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_02 		= 58
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_03 		= 59
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_04 		= 60
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_05 		= 61
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_06 		= 62
QuestManager.quests.FS_REFLEX_FETCH_QUEST_00 		= 63
QuestManager.quests.FS_REFLEX_FETCH_QUEST_01 		= 64
QuestManager.quests.FS_REFLEX_FETCH_QUEST_02 		= 65
QuestManager.quests.FS_REFLEX_FETCH_QUEST_03 		= 66
QuestManager.quests.FS_REFLEX_FETCH_QUEST_04 		= 67
QuestManager.quests.FS_REFLEX_FETCH_QUEST_05 		= 68
QuestManager.quests.FS_REFLEX_FETCH_QUEST_06 		= 69
QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_00 		= 70
QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_01 		= 71
QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_02 		= 72
QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_03 		= 73
QuestManager.quests.OLD_MAN_INITIAL 				= 74
QuestManager.quests.FS_THEATER_CAMP 				= 75
QuestManager.quests.DO_NOT_USE_BAD_SLOT 			= 76
QuestManager.quests.FS_GOTO_DATH 					= 77
QuestManager.quests.FS_VILLAGE_ELDER 				= 78
QuestManager.quests.OLD_MAN_FORCE_CRYSTAL 			= 79
QuestManager.quests.FS_DATH_WOMAN 					= 80
QuestManager.quests.FS_PATROL_QUEST_1 				= 81
QuestManager.quests.FS_PATROL_QUEST_2 				= 82
QuestManager.quests.FS_PATROL_QUEST_3 				= 83
QuestManager.quests.FS_PATROL_QUEST_4 				= 84
QuestManager.quests.FS_PATROL_QUEST_5 				= 85
QuestManager.quests.FS_PATROL_QUEST_6 				= 86
QuestManager.quests.FS_PATROL_QUEST_7 				= 87
QuestManager.quests.FS_PATROL_QUEST_8 				= 88
QuestManager.quests.FS_PATROL_QUEST_9 				= 89
QuestManager.quests.FS_PATROL_QUEST_10 				= 90
QuestManager.quests.FS_PATROL_QUEST_11 				= 91
QuestManager.quests.FS_PATROL_QUEST_12 				= 92
QuestManager.quests.FS_PATROL_QUEST_13 				= 93
QuestManager.quests.FS_PATROL_QUEST_14 				= 94
QuestManager.quests.FS_PATROL_QUEST_15 				= 95
QuestManager.quests.FS_PATROL_QUEST_16 				= 96
QuestManager.quests.FS_PATROL_QUEST_17 				= 97
QuestManager.quests.FS_PATROL_QUEST_18 				= 98
QuestManager.quests.FS_PATROL_QUEST_19 				= 99
QuestManager.quests.FS_PATROL_QUEST_20 				= 100
QuestManager.quests.FS_COMBAT_HEALING_1 			= 101
QuestManager.quests.FS_COMBAT_HEALING_2 			= 102
QuestManager.quests.FS_DEFEND_SET_FACTION 			= 103
QuestManager.quests.FS_DEFEND_01 					= 104
QuestManager.quests.FS_DEFEND_02 					= 105
QuestManager.quests.FS_DEFEND_REwARD_01 			= 106
QuestManager.quests.FS_DEFEND_03 					= 107
QuestManager.quests.FS_DEFEND_04 					= 108
QuestManager.quests.FS_CS_INTRO 					= 109
QuestManager.quests.FS_CS_KILL5_GUARDS 				= 110
QuestManager.quests.FS_CS_ENSURE_CAPTURE 			= 111
QuestManager.quests.FS_CS_LAST_CHANCE 				= 112
QuestManager.quests.FS_CS_ESCORT_COMMANDER_PRI 		= 113
QuestManager.quests.FS_CS_ESCORT_COMMANDER_SEC 		= 114
QuestManager.quests.FS_CS_QUEST_DONE 				= 115
QuestManager.quests.FS_THEATER_FINAL 				= 116
QuestManager.quests.OLD_MAN_FINAL 					= 117
QuestManager.quests.FS_CRAFTING4_QUEST_00 			= 118
QuestManager.quests.FS_CRAFTING4_QUEST_01 			= 119
QuestManager.quests.FS_CRAFTING4_QUEST_02 			= 120
QuestManager.quests.FS_CRAFTING4_QUEST_03 			= 121
QuestManager.quests.FS_CRAFTING4_QUEST_04 			= 122
QuestManager.quests.FS_CRAFTING4_QUEST_05 			= 123
QuestManager.quests.FS_CRAFTING4_QUEST_06 			= 124
QuestManager.quests.FS_CRAFTING4_QUEST_07 			= 125
QuestManager.quests.TWO_MILITARY 					= 126
QuestManager.quests.FS_DEFEND_REwARD_02 			= 127
QuestManager.quests.FS_DEFEND_REwARD_03 			= 128
QuestManager.quests.SURVEY_PHASE2_MAIN 				= 129
QuestManager.quests.SURVEY_PHASE2_01 				= 130
QuestManager.quests.SURVEY_PHASE2_02 				= 131
QuestManager.quests.SURVEY_PHASE2_03 				= 132
QuestManager.quests.SURVEY_PHASE2_04 				= 133
QuestManager.quests.SURVEY_PHASE2_05 				= 134
QuestManager.quests.SURVEY_PHASE2_06 				= 135
QuestManager.quests.SURVEY_PHASE2_07 				= 136
QuestManager.quests.SURVEY_PHASE2_08 				= 137
QuestManager.quests.SURVEY_PHASE3_MAIN 				= 138
QuestManager.quests.SURVEY_PHASE3_01 				= 139
QuestManager.quests.SURVEY_PHASE3_02 				= 140
QuestManager.quests.SURVEY_PHASE3_03 				= 141
QuestManager.quests.SURVEY_PHASE3_04 				= 142
QuestManager.quests.SURVEY_PHASE3_05 				= 143
QuestManager.quests.SURVEY_PHASE3_06 				= 144
QuestManager.quests.SURVEY_PHASE3_07 				= 145
QuestManager.quests.SURVEY_PHASE3_08 				= 146
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_01 	= 147
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_02 	= 148
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_03 	= 149
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_04 	= 150
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_05 	= 151
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_06 	= 152
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_07 	= 153
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_08 	= 154
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_09 	= 155
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_10 	= 156
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_11 	= 157
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_12 	= 158
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_13 	= 159
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_14 	= 160
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_15 	= 161
QuestManager.quests.FS_SURVEY_SPECIAL_RESOURCE_16 	= 162
QuestManager.quests.FS_DATH_wOMAN_TALK 				= 163
QuestManager.quests.FS_PATROL_QUEST_START 			= 164
QuestManager.quests.FS_REFLEX_RESCUE_QUEST_07 		= 165
QuestManager.quests.FS_SURVEY_PHASE2_REwARD 		= 166
QuestManager.quests.FS_SURVEY_PHASE3_REwARD 		= 167
QuestManager.quests.FS_DEFEND_SET_FACTION_02 		= 168
QuestManager.quests.LOOT_DATAPAD_1 					= 169
QuestManager.quests.GOT_DATAPAD 					= 170
QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_01 	= 171
QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_01 	= 172
QuestManager.quests.FS_PHASE_2_CRAFT_DEFENSES_MAIN 	= 173
QuestManager.quests.FS_PHASE_3_CRAFT_SHIELDS_MAIN 	= 174
QuestManager.quests.LOOT_DATAPAD_2 					= 175
QuestManager.quests.GOT_DATAPAD_2 					= 176
QuestManager.quests.FS_CS_QUEST_FAILED_ESCORT 		= 177
QuestManager.quests.FS_PATROL_QUEST_FINISH 			= 178
QuestManager.quests.FS_MEDIC_PUZZLE_QUEST_FINISH 	= 179
QuestManager.quests.FS_COMBAT_HEALING_FINISH 		= 180
QuestManager.quests.FS_COMBAT_REWARD_PHASE2 		= 181
QuestManager.quests.FS_REFLEX_REWARD_PHASE3 		= 182
QuestManager.quests.FS_DEFEND_WAIT_01 				= 183
QuestManager.quests.FS_DEFEND_WAIT_02 				= 184
QuestManager.quests.FS_CRAFTING4_QUEST_FINISH 		= 185
QuestManager.quests.FS_CRAFT_PUZZLE_QUEST_04 		= 186
QuestManager.quests.FS_CS_QUEST_DONE_NOTIFYONLY 	= 187
QuestManager.quests.CDF_LOST_PATROL					= 188
QuestManager.quests.CDF_LOST_PATROL_MAIN			= 189
QuestManager.quests.CDF_LOST_PATROL_RETURN			= 190
QuestManager.quests.CDF_COMMUNICATION				= 191
QuestManager.quests.CDF_COMMUNICATION_START			= 192
QuestManager.quests.CDF_COMMUNICATION_01			= 193
QuestManager.quests.CDF_COMMUNICATION_02			= 194
QuestManager.quests.CDF_CONTROL_TERMINALS			= 195
QuestManager.quests.CDF_CONTROL_TERMINALS_01		= 196
QuestManager.quests.CDF_CONTROL_TERMINALS_02		= 197
QuestManager.quests.CDF_CONTROL_TERMINALS_03		= 198
QuestManager.quests.CDF_CONTROL_TERMINALS_04		= 199
QuestManager.quests.CDF_DISABLE_FACTORY				= 200
QuestManager.quests.CDF_DISABLE_FACTORY_MAIN		= 201
QuestManager.quests.CDF_DISABLE_FACTORY_RETURN		= 202
QuestManager.quests.CDF_DESTROY						= 203
QuestManager.quests.CDF_DESTROY_01					= 204
QuestManager.quests.CDF_DESTROY_02					= 205
QuestManager.quests.CDF_DESTROY_03					= 206
QuestManager.quests.CDF_DESTROY_04					= 207
QuestManager.quests.CDF_DESTROY_RETURN				= 208
QuestManager.quests.SEA_REMOVAL_TOOL				= 209
QuestManager.quests.SEA_REMOVAL_TOOL_01				= 210
QuestManager.quests.SEA_REMOVAL_TOOL_02				= 211
QuestManager.quests.SEA_REMOVAL_TOOL_03				= 212
QuestManager.quests.SEA_REMOVAL_TOOL_04				= 213
QuestManager.quests.SEA_REMOVAL_TOOL_05				= 214
QuestManager.quests.SEA_REMOVAL_TOOL_06				= 215
QuestManager.quests.SEA_REMOVAL_TOOL_07				= 216
QuestManager.quests.SEA_REMOVAL_TOOL_08				= 217
QuestManager.quests.SEA_REMOVAL_TOOL_09				= 218
QuestManager.quests.SEA_REMOVAL_TOOL_COMPONENT		= 219
QuestManager.quests.SEA_REMOVAL_TOOL_COMPONENT_01	= 220
QuestManager.quests.SEA_REMOVAL_TOOL_COMPONENT_02	= 221
QuestManager.quests.DUSTDURNI						= 222
QuestManager.quests.DUSTDURNI_01					= 223
QuestManager.quests.DUSTDURNI_02					= 224
QuestManager.quests.DUSTDURNI_03					= 225
QuestManager.quests.DUSTDURNI_04					= 226
QuestManager.quests.DUSTDURNI_05					= 227
QuestManager.quests.DUSTDURNI_06					= 228
QuestManager.quests.DUSTDURNI_07					= 229
QuestManager.quests.DUSTDURNI_08					= 230
QuestManager.quests.DUSTDURNI_09					= 231
QuestManager.quests.DUSTDURNI_10					= 232
QuestManager.quests.DUSTDURNI_11					= 233
QuestManager.quests.PURPLEFLOWER					= 234
QuestManager.quests.PURPLEFLOWER_01					= 235
QuestManager.quests.PURPLEFLOWER_02					= 236
QuestManager.quests.PURPLEFLOWER_03					= 237
QuestManager.quests.PURPLEFLOWER_04					= 238
QuestManager.quests.PURPLEFLOWER_05					= 239
QuestManager.quests.PURPLEFLOWER_06					= 240
QuestManager.quests.PURPLEFLOWER_07					= 241
QuestManager.quests.PURPLEFLOWER_08					= 242
QuestManager.quests.PURPLEFLOWER_09					= 243
QuestManager.quests.PURPLEFLOWER_10					= 244
QuestManager.quests.PURPLEFLOWER_11					= 245
QuestManager.quests.PURPLEFLOWER_12					= 246
QuestManager.quests.PURPLEFLOWER_13					= 247
QuestManager.quests.PURPLEFLOWER_14					= 248
QuestManager.quests.PURPLEFLOWER_15					= 249
QuestManager.quests.PURPLEFLOWER_16					= 250
QuestManager.quests.PURPLEFLOWER_17					= 251
QuestManager.quests.PURPLEFLOWER_18					= 252
QuestManager.quests.PURPLEFLOWER_19					= 253
QuestManager.quests.PURPLEFLOWER_20					= 254
QuestManager.quests.PURPLEFLOWER_21					= 255
QuestManager.quests.BEETLES							= 256
QuestManager.quests.BEETLES_01						= 257
QuestManager.quests.BEETLES_02						= 258
QuestManager.quests.BEETLES_03						= 259
QuestManager.quests.BEETLES_04						= 260
QuestManager.quests.BEETLES_05						= 261
QuestManager.quests.BEETLES_06						= 262
QuestManager.quests.BEETLES_07						= 263
QuestManager.quests.INSTRUMENTS						= 264
QuestManager.quests.INSTRUMENTS_01					= 265
QuestManager.quests.INSTRUMENTS_02					= 266
QuestManager.quests.INSTRUMENTS_03					= 267
QuestManager.quests.INSTRUMENTS_04					= 268
QuestManager.quests.INSTRUMENTS_05					= 269
QuestManager.quests.INSTRUMENTS_06					= 270
QuestManager.quests.INSTRUMENTS_07					= 271
QuestManager.quests.VISTAS							= 272
QuestManager.quests.VISTAS_01						= 273
QuestManager.quests.VISTAS_02						= 274
QuestManager.quests.VISTAS_03						= 275
QuestManager.quests.VISTAS_04						= 276
QuestManager.quests.VISTAS_05						= 277
QuestManager.quests.VISTAS_06						= 278
QuestManager.quests.VISTAS_07						= 279
QuestManager.quests.VISTAS_08						= 280
QuestManager.quests.VISTAS_09						= 281
QuestManager.quests.VISTAS_10						= 282
QuestManager.quests.VISTAS_11						= 283
QuestManager.quests.HALLOWEEN_DAILY					= 284
QuestManager.quests.HALLOWEEN_DAILY_RETURN			= 285
QuestManager.quests.BIRD_BUTCHER_01					= 286
QuestManager.quests.BIRD_BUTCHER_01_01				= 287
QuestManager.quests.BIRD_BUTCHER_01_02				= 288
QuestManager.quests.BIRD_BUTCHER_01_03				= 289
QuestManager.quests.BIRD_BUTCHER_01_04				= 290
QuestManager.quests.BIRD_BUTCHER_02					= 291
QuestManager.quests.BIRD_BUTCHER_02_01				= 292
QuestManager.quests.BIRD_BUTCHER_02_02				= 293
QuestManager.quests.BIRD_BUTCHER_02_03				= 294
QuestManager.quests.BIRD_BUTCHER_03					= 295
QuestManager.quests.BIRD_BUTCHER_03_01				= 296
QuestManager.quests.BIRD_BUTCHER_03_02				= 297
QuestManager.quests.BIRD_BUTCHER_03_03				= 298
QuestManager.quests.BIRD_BUTCHER_04					= 299
QuestManager.quests.BIRD_BUTCHER_04_01				= 300
QuestManager.quests.BIRD_BUTCHER_04_02				= 301
QuestManager.quests.BIRD_BUTCHER_04_03				= 302
QuestManager.quests.BIRD_BUTCHER_04_04				= 303
QuestManager.quests.BIRD_BUTCHER_04_05				= 304
QuestManager.quests.BIRD_BUTCHER_05					= 305
QuestManager.quests.BIRD_BUTCHER_05_01				= 306
QuestManager.quests.BIRD_BUTCHER_05_02				= 307
QuestManager.quests.BIRD_BUTCHER_05_03				= 308
QuestManager.quests.BIRD_BUTCHER_06					= 309
QuestManager.quests.BIRD_BUTCHER_06_01				= 310
QuestManager.quests.BIRD_BUTCHER_06_02				= 311
QuestManager.quests.BIRD_BUTCHER_06_03				= 312
QuestManager.quests.BIRD_BUTCHER_06_04				= 313
QuestManager.quests.BIRD_BUTCHER_06_05				= 314
QuestManager.quests.COLD_BLOODED_KILLER_01			= 315
QuestManager.quests.COLD_BLOODED_KILLER_01_01		= 316
QuestManager.quests.COLD_BLOODED_KILLER_01_02		= 317
QuestManager.quests.COLD_BLOODED_KILLER_01_03		= 318
QuestManager.quests.COLD_BLOODED_KILLER_01_04		= 319
QuestManager.quests.COLD_BLOODED_KILLER_01_05		= 320
QuestManager.quests.COLD_BLOODED_KILLER_02			= 321
QuestManager.quests.COLD_BLOODED_KILLER_02_01		= 322
QuestManager.quests.COLD_BLOODED_KILLER_02_02		= 323
QuestManager.quests.COLD_BLOODED_KILLER_02_03		= 324
QuestManager.quests.COLD_BLOODED_KILLER_02_04		= 325
QuestManager.quests.COLD_BLOODED_KILLER_02_05		= 326
QuestManager.quests.COLD_BLOODED_KILLER_03			= 327
QuestManager.quests.COLD_BLOODED_KILLER_03_01		= 328
QuestManager.quests.COLD_BLOODED_KILLER_03_02		= 329
QuestManager.quests.COLD_BLOODED_KILLER_03_03		= 330
QuestManager.quests.COLD_BLOODED_KILLER_03_04		= 331
QuestManager.quests.COLD_BLOODED_KILLER_03_05		= 332
QuestManager.quests.COLD_BLOODED_KILLER_04			= 333
QuestManager.quests.COLD_BLOODED_KILLER_04_01		= 334
QuestManager.quests.COLD_BLOODED_KILLER_04_02		= 335
QuestManager.quests.COLD_BLOODED_KILLER_04_03		= 336
QuestManager.quests.COLD_BLOODED_KILLER_04_04		= 337
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01	= 338
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01_01	= 339
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01_02	= 340
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01_03	= 341
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01_04	= 342
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_01_05	= 343
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02	= 344
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02_01	= 345
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02_02	= 346
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02_03	= 347
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02_04	= 348
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_02_05	= 349
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03	= 350
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03_01	= 351
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03_02	= 352
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03_03	= 353
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03_04	= 354
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_03_05	= 355
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_04	= 356
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_04_01	= 357
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_04_02	= 358
QuestManager.quests.FEROCIOUS_CREATURE_SLAYER_04_03	= 359
QuestManager.quests.INSECT_EXTERMINATOR_01			= 360
QuestManager.quests.INSECT_EXTERMINATOR_01_01		= 361
QuestManager.quests.INSECT_EXTERMINATOR_01_02		= 362
QuestManager.quests.INSECT_EXTERMINATOR_01_03		= 363
QuestManager.quests.INSECT_EXTERMINATOR_01_04		= 364
QuestManager.quests.INSECT_EXTERMINATOR_01_05		= 365
QuestManager.quests.INSECT_EXTERMINATOR_02			= 366
QuestManager.quests.INSECT_EXTERMINATOR_02_01		= 367
QuestManager.quests.INSECT_EXTERMINATOR_02_02		= 368
QuestManager.quests.INSECT_EXTERMINATOR_02_03		= 369
QuestManager.quests.INSECT_EXTERMINATOR_02_04		= 370
QuestManager.quests.INSECT_EXTERMINATOR_02_05		= 371
QuestManager.quests.INSECT_EXTERMINATOR_03			= 372
QuestManager.quests.INSECT_EXTERMINATOR_03_01		= 373
QuestManager.quests.INSECT_EXTERMINATOR_03_02		= 374
QuestManager.quests.INSECT_EXTERMINATOR_03_03		= 375
QuestManager.quests.INSECT_EXTERMINATOR_03_04		= 376
QuestManager.quests.INSECT_EXTERMINATOR_04			= 377
QuestManager.quests.INSECT_EXTERMINATOR_04_01		= 378
QuestManager.quests.INSECT_EXTERMINATOR_04_02		= 379
QuestManager.quests.INSECT_EXTERMINATOR_04_03		= 380
QuestManager.quests.INSECT_EXTERMINATOR_04_04		= 381
QuestManager.quests.INSECT_EXTERMINATOR_04_05		= 382
QuestManager.quests.INSECT_EXTERMINATOR_05			= 383
QuestManager.quests.INSECT_EXTERMINATOR_05_01		= 384
QuestManager.quests.INSECT_EXTERMINATOR_05_02		= 385
QuestManager.quests.INSECT_EXTERMINATOR_05_03		= 386
QuestManager.quests.INSECT_EXTERMINATOR_05_04		= 387
QuestManager.quests.INSECT_EXTERMINATOR_05_05		= 388
QuestManager.quests.INSECT_EXTERMINATOR_06			= 389
QuestManager.quests.INSECT_EXTERMINATOR_06_01		= 390
QuestManager.quests.INSECT_EXTERMINATOR_06_02		= 391
QuestManager.quests.INSECT_EXTERMINATOR_06_03		= 392
QuestManager.quests.INSECT_EXTERMINATOR_06_04		= 393
QuestManager.quests.INSECT_EXTERMINATOR_06_05		= 394
QuestManager.quests.INSECT_EXTERMINATOR_07			= 395
QuestManager.quests.INSECT_EXTERMINATOR_07_01		= 396
QuestManager.quests.INSECT_EXTERMINATOR_07_02		= 397
QuestManager.quests.INSECT_EXTERMINATOR_07_03		= 398
QuestManager.quests.INSECT_EXTERMINATOR_07_04		= 399
QuestManager.quests.INSECT_EXTERMINATOR_07_05		= 400
QuestManager.quests.INSECT_EXTERMINATOR_08			= 401
QuestManager.quests.INSECT_EXTERMINATOR_08_01		= 402
QuestManager.quests.INSECT_EXTERMINATOR_08_02		= 403
QuestManager.quests.INSECT_EXTERMINATOR_08_03		= 404
QuestManager.quests.INSECT_EXTERMINATOR_08_04		= 405
QuestManager.quests.INSECT_EXTERMINATOR_08_05		= 406
QuestManager.quests.INSECT_EXTERMINATOR_09			= 407
QuestManager.quests.INSECT_EXTERMINATOR_09_01		= 408
QuestManager.quests.INSECT_EXTERMINATOR_09_02		= 409
QuestManager.quests.INSECT_EXTERMINATOR_09_03		= 410
QuestManager.quests.INSECT_EXTERMINATOR_09_04		= 411
QuestManager.quests.INSECT_EXTERMINATOR_09_05		= 412
QuestManager.quests.PRIMATE_PLUNDERER_01			= 413
QuestManager.quests.PRIMATE_PLUNDERER_01_01			= 414
QuestManager.quests.PRIMATE_PLUNDERER_01_02			= 415
QuestManager.quests.PRIMATE_PLUNDERER_01_03			= 416
QuestManager.quests.PRIMATE_PLUNDERER_01_04			= 417
QuestManager.quests.PRIMATE_PLUNDERER_01_05			= 418
QuestManager.quests.PRIMATE_PLUNDERER_02			= 419
QuestManager.quests.PRIMATE_PLUNDERER_02_01			= 420
QuestManager.quests.PRIMATE_PLUNDERER_02_02			= 421
QuestManager.quests.PRIMATE_PLUNDERER_02_03			= 422
QuestManager.quests.PRIMATE_PLUNDERER_02_04			= 423
QuestManager.quests.PRIMATE_PLUNDERER_02_05			= 424
QuestManager.quests.PRIMATE_PLUNDERER_03			= 425
QuestManager.quests.PRIMATE_PLUNDERER_03_01			= 426
QuestManager.quests.PRIMATE_PLUNDERER_03_02			= 427
QuestManager.quests.PRIMATE_PLUNDERER_03_03			= 428
QuestManager.quests.PRIMATE_PLUNDERER_03_04			= 429
QuestManager.quests.PRIMATE_PLUNDERER_03_05			= 430
QuestManager.quests.PRIMATE_PLUNDERER_04			= 431
QuestManager.quests.PRIMATE_PLUNDERER_04_01			= 432
QuestManager.quests.PRIMATE_PLUNDERER_04_02			= 433
QuestManager.quests.PRIMATE_PLUNDERER_04_03			= 434
QuestManager.quests.PRIMATE_PLUNDERER_04_04			= 435
QuestManager.quests.PRIMATE_PLUNDERER_04_05			= 436
QuestManager.quests.PRIMATE_PLUNDERER_05			= 437
QuestManager.quests.PRIMATE_PLUNDERER_05_01			= 438
QuestManager.quests.PRIMATE_PLUNDERER_05_02			= 439
QuestManager.quests.PRIMATE_PLUNDERER_05_03			= 440
QuestManager.quests.PRIMATE_PLUNDERER_05_04			= 441
QuestManager.quests.PRIMATE_PLUNDERER_05_05			= 442
QuestManager.quests.PRIMATE_PLUNDERER_06			= 443
QuestManager.quests.PRIMATE_PLUNDERER_06_01			= 444
QuestManager.quests.PRIMATE_PLUNDERER_06_02			= 445
QuestManager.quests.PRIMATE_PLUNDERER_06_03			= 446
QuestManager.quests.PRIMATE_PLUNDERER_06_04			= 447
QuestManager.quests.PRIMATE_PLUNDERER_06_05			= 448
QuestManager.quests.RODENT_EXTERMINATOR_01			= 449
QuestManager.quests.RODENT_EXTERMINATOR_01_01		= 450
QuestManager.quests.RODENT_EXTERMINATOR_01_02		= 451
QuestManager.quests.RODENT_EXTERMINATOR_01_03		= 452
QuestManager.quests.RODENT_EXTERMINATOR_01_04		= 453
QuestManager.quests.RODENT_EXTERMINATOR_01_05		= 454
QuestManager.quests.RODENT_EXTERMINATOR_02			= 455
QuestManager.quests.RODENT_EXTERMINATOR_02_01		= 456
QuestManager.quests.RODENT_EXTERMINATOR_02_02		= 457
QuestManager.quests.RODENT_EXTERMINATOR_02_03		= 458
QuestManager.quests.RODENT_EXTERMINATOR_02_04		= 459
QuestManager.quests.RODENT_EXTERMINATOR_02_05		= 460
QuestManager.quests.RODENT_EXTERMINATOR_03			= 461
QuestManager.quests.RODENT_EXTERMINATOR_03_01		= 462
QuestManager.quests.RODENT_EXTERMINATOR_03_02		= 463
QuestManager.quests.RODENT_EXTERMINATOR_03_03		= 464
QuestManager.quests.RODENT_EXTERMINATOR_03_04		= 465
QuestManager.quests.RODENT_EXTERMINATOR_03_05		= 466
QuestManager.quests.RODENT_EXTERMINATOR_04			= 467
QuestManager.quests.RODENT_EXTERMINATOR_04_01		= 468
QuestManager.quests.RODENT_EXTERMINATOR_04_02		= 469
QuestManager.quests.RODENT_EXTERMINATOR_04_03		= 470
QuestManager.quests.RODENT_EXTERMINATOR_04_04		= 471
QuestManager.quests.RODENT_EXTERMINATOR_04_05		= 472
QuestManager.quests.RODENT_EXTERMINATOR_05			= 473
QuestManager.quests.RODENT_EXTERMINATOR_05_01		= 474
QuestManager.quests.RODENT_EXTERMINATOR_05_02		= 475
QuestManager.quests.RODENT_EXTERMINATOR_05_03		= 476
QuestManager.quests.RODENT_EXTERMINATOR_05_04		= 477
QuestManager.quests.RODENT_EXTERMINATOR_05_05		= 478
QuestManager.quests.RODENT_EXTERMINATOR_06			= 479
QuestManager.quests.RODENT_EXTERMINATOR_06_01		= 480
QuestManager.quests.RODENT_EXTERMINATOR_06_02		= 481
QuestManager.quests.RODENT_EXTERMINATOR_06_03		= 482
QuestManager.quests.RODENT_EXTERMINATOR_06_04		= 483
QuestManager.quests.RODENT_EXTERMINATOR_06_05		= 484
QuestManager.quests.RODENT_EXTERMINATOR_07			= 485
QuestManager.quests.RODENT_EXTERMINATOR_07_01		= 486
QuestManager.quests.RODENT_EXTERMINATOR_07_02		= 487
QuestManager.quests.RODENT_EXTERMINATOR_07_03		= 488
QuestManager.quests.RODENT_EXTERMINATOR_07_04		= 489
QuestManager.quests.RODENT_EXTERMINATOR_08			= 490
QuestManager.quests.RODENT_EXTERMINATOR_08_01		= 491
QuestManager.quests.RODENT_EXTERMINATOR_08_02		= 492
QuestManager.quests.RODENT_EXTERMINATOR_08_03		= 493
QuestManager.quests.RODENT_EXTERMINATOR_08_04		= 494
QuestManager.quests.RODENT_EXTERMINATOR_08_05		= 495
QuestManager.quests.RODENT_EXTERMINATOR_09			= 496
QuestManager.quests.RODENT_EXTERMINATOR_09_01		= 497
QuestManager.quests.RODENT_EXTERMINATOR_09_02		= 498
QuestManager.quests.RODENT_EXTERMINATOR_09_03		= 499
QuestManager.quests.RODENT_EXTERMINATOR_09_04		= 500
QuestManager.quests.RODENT_EXTERMINATOR_10			= 501
QuestManager.quests.RODENT_EXTERMINATOR_10_01		= 502
QuestManager.quests.RODENT_EXTERMINATOR_10_02		= 503
QuestManager.quests.RODENT_EXTERMINATOR_10_03		= 504
QuestManager.quests.RODENT_EXTERMINATOR_10_04		= 505
QuestManager.quests.RODENT_EXTERMINATOR_10_05		= 506
QuestManager.quests.DUPLICATOR								= 507
QuestManager.quests.DUPLICATOR_01							= 508
QuestManager.quests.DUPLICATOR_02							= 509
QuestManager.quests.DUPLICATOR_03							= 510
QuestManager.quests.DUPLICATOR_04							= 511
QuestManager.quests.DUPLICATOR_05							= 512
QuestManager.quests.DUPLICATOR_06							= 513
QuestManager.quests.DUPLICATOR_07							= 514
QuestManager.quests.DUPLICATOR_08							= 515
QuestManager.quests.DUPLICATOR_09							= 516
QuestManager.quests.HELLFIREBPH								= 517
QuestManager.quests.HELLFIREBPH_01						= 518
QuestManager.quests.HELLFIREBPH_02						= 519
QuestManager.quests.HELLFIREBPH_03						= 520
QuestManager.quests.HELLFIREBPH_04						= 521
QuestManager.quests.HELLFIREBPH_05						= 522
QuestManager.quests.HELLFIREBPH_06						= 523
QuestManager.quests.HELLFIREBPH_07						= 524
QuestManager.quests.TSEFUELCELL							  = 525
QuestManager.quests.TSEFUELCELL_01						= 526
QuestManager.quests.TSEFUELCELL_02						= 527
QuestManager.quests.TSEFUELCELL_03						= 528
QuestManager.quests.TSEFUELCELL_04						= 529
QuestManager.quests.TSEFUELCELL_05						= 530
QuestManager.quests.TSEFUELCELL_06						= 531
QuestManager.quests.TSEFUELCELL_07						= 532
QuestManager.quests.SARLACCCHOW							  = 533
QuestManager.quests.SARLACCCHOW_01						= 534
QuestManager.quests.SARLACCCHOW_02						= 535
QuestManager.quests.SARLACCCHOW_03						= 536
QuestManager.quests.SARLACCCHOW_04						= 537
QuestManager.quests.SARLACCCHOW_05						= 538
QuestManager.quests.SARLACCCHOW_06						= 539
QuestManager.quests.SARLACCCHOW_07						= 540

--[[
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_MAIN		= 188
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_01		= 189
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_02		= 190
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_03		= 191
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_04		= 192
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_05		= 193
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_06		= 194
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_07		= 195
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_08		= 196
QuestManager.quests.WOD_PROLOGUE_WALKABOUT_09		= 197
QuestManager.quests.WOD_PROLOGUE_MEET_MYSTERIOUS_WITCH	= 198]]

return QuestManager
