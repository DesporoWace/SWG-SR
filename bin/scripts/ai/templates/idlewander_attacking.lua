idlewander_attacking = {
	{"root", "CompositeDefault", "none", SELECTORBEHAVIOR},
	{"attack", "Composite", "root", SEQUENCEBEHAVIOR},
	{"move", "CompositeDefault", "root", SEQUENCEBEHAVIOR},
	{"patrol", "GeneratePatrolDefault", "root", BEHAVIOR},
	{"attack0", "GetTarget", "attack", BEHAVIOR},
	{"attack1", "SelectWeapon", "attack", BEHAVIOR},
	{"attack2", "SelectAttack", "attack", BEHAVIOR},
	{"attack3", "CombatMove", "attack", BEHAVIOR},
	{"move0", "WalkDefault", "move", BEHAVIOR},
	{"move1", "Wait10Default", "move", BEHAVIOR},
}

addAiTemplate("idlewander_attacking", idlewander_attacking)

idlewander_attackingpack = {
	{"idroot", "CompositePack", "none", SELECTORBEHAVIOR},
	{"move", "CompositePack", "idroot", SEQUENCEBEHAVIOR},
	{"patrol", "GeneratePatrolPack", "idroot", BEHAVIOR},
	{"move0", "WalkPack", "move", BEHAVIOR},
	{"move1", "Wait10Pack", "move", BEHAVIOR},
}

addAiTemplate("idlewander_attackingpack", idlewander_attackingpack)

idlewander_attackingstatic = {
	{"idroot", "CompositeDefault", "none", SELECTORBEHAVIOR},
	{"move0", "WalkDefault", "idroot", BEHAVIOR},
	{"move1", "Wait10Default", "idroot", BEHAVIOR},
}

addAiTemplate("idlewander_attackingstatic", idlewander_attackingstatic)
