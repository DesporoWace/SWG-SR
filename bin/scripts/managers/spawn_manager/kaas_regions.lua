require("scripts.managers.spawn_manager.regions")

kaas_regions = {
  {"a_rebel_outpost",-6131,2705,{CIRCLE,700},NOSPAWNAREA + NOBUILDZONEAREA},  -- kaas city
  {"a_rebel_outpost_01",-6131,2705,{CIRCLE,2000},NOBUILDZONEAREA},  -- kaas city

  {"borgle_bat_cave",2850,3890,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA}, -- smuggler op
  {"borgle_bat_cave_01",2850,3890,{CIRCLE,700},NOBUILDZONEAREA}, -- smuggler op
  {"borgle_bat_cave_02",-5117,-2293,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA}, -- garrison
  {"borgle_bat_cave_03",-5117,-2293,{CIRCLE,1500},NOBUILDZONEAREA}, -- garrison

  {"camp_and_BH",3342,2634,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA}, --gundark cave
  {"camp_and_BH_02",3342,2634,{CIRCLE,500},NOBUILDZONEAREA}, --gundark cave
  {"camp_and_BH_01",-6135,6575,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA}, --mailoc cave
  {"camp_and_BH_03",-6135,6575,{CIRCLE,500},NOBUILDZONEAREA}, --mailoc cave

  {"camp_and_skeleton",6017,-1141,{CIRCLE,400},NOSPAWNAREA + NOBUILDZONEAREA}, --tomb of vitiate

  {"cobral_tent",-70,6370,{CIRCLE,100},NOSPAWNAREA + NOBUILDZONEAREA}, -- ruined sith complex
  {"cobral_tent_01",-70,6370,{CIRCLE,1000},NOBUILDZONEAREA}, -- ruined sith complex
  {"cobral_tent_02",-1119,-4767,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA}, -- Dark Force Shrine
  {"cobral_tent_03",-1119,-4767,{CIRCLE,1000},NOBUILDZONEAREA}, -- Dark Force Shrine

  {"narmle_easy_newbie",-5205,-2290,{CIRCLE,1400},SPAWNAREA,{"kaas_world"},256},
  {"narmle_medium_newbie",-5200,-2290,{RING,1400,2200},SPAWNAREA,{"kaas_world"},384},

  {"rebel_outpost",3677,-6447,{CIRCLE,1500},SPAWNAREA,{"kaas_world"},384},
  {"restuss",0,0,{CIRCLE,0},UNDEFINEDAREA},
  {"restuss_easy_newbie",5300,5700,{CIRCLE,1400},SPAWNAREA,{"kaas_world"},256},
  {"restuss_medium_newbie",5305,5700,{RING,1400,2200},SPAWNAREA,{"kaas_world"},384},

  {"world_spawner",0,0,{CIRCLE,-1},SPAWNAREA + WORLDSPAWNAREA,{"kaas_world","global"},2048},

}

