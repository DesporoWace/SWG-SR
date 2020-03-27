require("scripts.managers.spawn_manager.regions")

taanab_regions = {
	{"pandath",2000,5600,{RECTANGLE,400,1300},NOSPAWNAREA + NOBUILDZONEAREA},
	{"starhunterstation",3763,-5425,{CIRCLE,300},NOSPAWNAREA + NOBUILDZONEAREA},
	{"world_spawner",0,0,{CIRCLE,-1},SPAWNAREA + WORLDSPAWNAREA,{"taanab_world"},2048},
	{"taanabhexfarms",-3000,-105,{CIRCLE,300},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabgreatherdnbz",5537,-4958,{CIRCLE,400},NOBUILDZONEAREA},
	{"taanabgreatherd",5537,-4958,{CIRCLE,300},NOWORLDSPAWNAREA + SPAWNAREA,{"taanab_nerfherd"},1024},
	{"downedship",3293,-1324,{CIRCLE,150},NOBUILDZONEAREA},
	{"taanabmine",-2609,-1305,{CIRCLE,200},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabcave",-850,7200,{CIRCLE,150},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabnorulaccave",-792,7165,{CIRCLE,150},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabdefenderoutpost",6230,3755,{CIRCLE,160},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabrobacave",5765,1277,{CIRCLE,100},NOSPAWNAREA + NOBUILDZONEAREA},
	{"tanaabvosstorehouse",-7090,4574,{CIRCLE,180},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabhydrostation",-3548,-4289,{CIRCLE,220},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabsurveyorscamp",1100,267,{CIRCLE,280},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabresearchfacility",5880,-3240,{CIRCLE,350},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabvistaplateau",-3922,3188,{CIRCLE,160},NOSPAWNAREA + NOBUILDZONEAREA},
	{"taanabcanyonlandsnorthnbz",-2672,4544,{CIRCLE,660},NOBUILDZONEAREA},
	{"taanabcanyonlandsnorth",-2672,4544,{CIRCLE,500},NOSPAWNAREA},
	{"taanabcanyonlandscenternbz",-2368,3424,{CIRCLE,660},NOBUILDZONEAREA},
	{"taanabcanyonlandscenter",-2368,3424,{CIRCLE,500},NOSPAWNAREA},
	{"taanabcanyonlandssouthnbz",-1600,2544,{CIRCLE,700},NOBUILDZONEAREA},
	{"taanabcanyonlandssouth",-1600,2544,{CIRCLE,560},NOSPAWNAREA},

	-- Unlisted POIs, use descriptive names
	{"taanabdeco_rockspire",-151,5253,{CIRCLE,16},NOBUILDZONEAREA},
	{"taanabdeco_jedirelic",-1853,5363,{CIRCLE,6},NOBUILDZONEAREA},
	{"taanabdeco_surivalistcamp",-4320,5390,{CIRCLE,64},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_tent",-3839,4033,{CIRCLE,20},NOBUILDZONEAREA},
	{"taanabdeco_skeletons",-3796,3682,{CIRCLE,10},NOBUILDZONEAREA},
	{"taanabdeco_brokenstuff",-4111,3750,{CIRCLE,6},NOBUILDZONEAREA},
	{"taanabdeco_drumskeleton",-5204,3022,{CIRCLE,8},NOBUILDZONEAREA},
	{"taanabdeco_tiedebris",-5519,1575,{CIRCLE,20},NOBUILDZONEAREA},
	{"taanabdeco_shipdebris",-5367,973,{CIRCLE,20},NOBUILDZONEAREA},
	{"taanabdeco_campfirerocks",810,-398,{CIRCLE,30},NOBUILDZONEAREA},
	{"taanabdeco_destroyedfarm",5428,-967,{CIRCLE,50},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_disabledspeeder",1785,-1088,{CIRCLE,16},NOBUILDZONEAREA},
	{"taanabdeco_brokenweapons",569,-1897,{CIRCLE,9},NOBUILDZONEAREA},
	{"taanabdeco_crashedlandspeeder",-627,-2324,{CIRCLE,50},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_destroyedjedichapterhouse",-1252,-3447,{CIRCLE,80},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_destroyedtreebuilding",-362,-3717,{CIRCLE,42},NOBUILDZONEAREA},
	{"taanabdeco_illegalranchers",1097,-3337,{CIRCLE,40},NOBUILDZONEAREA},
	{"taanabdeco_surveyors",6302,-4508,{CIRCLE,64},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_smallmininginstallation",-2251,552,{CIRCLE,40},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_vacationhome",-1485,2790,{CIRCLE,64},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_factorycorral",-5618,-4803,{CIRCLE,64},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_abandonedoutpost",-4404,-7144,{CIRCLE,180},NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"taanabdeco_tentarrangement",3925,1849,{CIRCLE,24},NOBUILDZONEAREA},

	{"taanabdeco_extinctvolcano",1174,-4006,{CIRCLE,80},NOBUILDZONEAREA + NOWORLDSPAWNAREA}, -- 2x tanrat
	{"taanabdeco_tanratskeleton",-6023,-5872,{CIRCLE,80},NOBUILDZONEAREA + NOWORLDSPAWNAREA}, -- 3x tanrat
	{"taanabdeco_bridge",553,1826,{CIRCLE,80},NOBUILDZONEAREA + NOWORLDSPAWNAREA}, -- 1x tanrat

 	-- Do not require no build
	--{"taanabdeco_crates",2003,6393,{CIRCLE,50},NOBUILDZONEAREA},
	--{"taanabdeco_wiring",1391,6062,{CIRCLE,50},NOBUILDZONEAREA},
	--{"taanabdeco_mushrooms",-3771,854,{CIRCLE,50},NOBUILDZONEAREA},
	--{"taanabdeco_powercrystal",-2649,-2499,{CIRCLE,50},NOBUILDZONEAREA},
	--{"taanabdeco_treefog",2360,-4238,{CIRCLE,50},NOBUILDZONEAREA},
	--{"taanabdeco_pillar",6949,-5858,{CIRCLE,50},NOBUILDZONEAREA},
}
