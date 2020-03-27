PlayerTriggers = { }

function PlayerTriggers:playerLoggedIn(pPlayer)
	if (pPlayer == nil) then
		return
	end
	ServerEventAutomation:playerLoggedIn(pPlayer)
	BestineElection:playerLoggedIn(pPlayer)
	droidFactorySlayer:playerLoggedIn(pPlayer)
	slayerCollections_birdButcher:playerLoggedIn(pPlayer)
	slayerCollections_coldBloodedKiller:playerLoggedIn(pPlayer)
	slayerCollections_ferociousCreatureSlayer:playerLoggedIn(pPlayer)
	slayerCollections_insectExterminator:playerLoggedIn(pPlayer)
	slayerCollections_primatePlunderer:playerLoggedIn(pPlayer)
	slayerCollections_rodentExterminator:playerLoggedIn(pPlayer)
end

function PlayerTriggers:playerLoggedOut(pPlayer)
	if (pPlayer == nil) then
		return
	end
	srHoloCostumeScreenplay:removeCostume(pPlayer)
	trickTreatScreenplay:removeCostume(pPlayer)
	ServerEventAutomation:playerLoggedOut(pPlayer)
	end
