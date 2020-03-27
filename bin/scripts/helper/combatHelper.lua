-- Author: Matthias Muente (matthias.muente@gmx.de)
-- https://revelationonline.net

-- Thanks HK!

local combatHelper = {}
local ObjectManager = require("managers.object.object_manager")

function combatHelper:checkCreatureDistance(x, z, y, maxDistance, pCreature)
    local ret = false
    ObjectManager.withSceneObject(pCreature, function(creature)
        if (creature:getDistanceToPosition(x, z, y) <= maxDistance) then
            ret = true
        end
    end)
    return ret
end

function combatHelper:checkCreatureWorldDistance(x, y, maxDistance, pCreature)
    local ret = false
    ObjectManager.withSceneObject(pCreature, function(creature)
        if (math.pow(creature:getWorldPositionX() - x, 2) + math.pow(creature:getWorldPositionY() - y,2)) <= math.pow(maxDistance,2) then
            ret = true
        end
    end)
    return ret
end

function combatHelper:healCreatureToMax(pCreature)
    ObjectManager.withCreatureObject(pCreature, function(creature)
        for i = 0 , 8 , 1 do
            creature:healDamage(pCreature, i, creature:getMaxHAM(i))
        end
    end)
    return 0
end

function combatHelper:healCreatureHAM(pCreature, value)
    ObjectManager.withCreatureObject(pCreature, function(creature)
        creature:healDamage(pCreature, 0, value)
        creature:healDamage(pCreature, 3, value)
        creature:healDamage(pCreature, 6, value)
    end)
end

function combatHelper:forcePeace(pCreature)
    forcePeace(pCreature)
    forcePeace(pCreature)
    return 0
end

function combatHelper:logicBossState(pBoss, percent, stateString, checkState)
    if ((SceneObject(pBoss) ~= nil) and (SceneObject(pBoss):getZoneName() ~= "")) then
        if (SceneObject(pBoss):isCreatureObject() and CreatureObject(pBoss):isInCombat() and not(CreatureObject(pBoss):isDead())) then
            if (stateString and checkState and (readData(stateString) == checkState)) then
                for i = 0, 6, 3 do
                    if (CreatureObject(pBoss):getHAM(i) <= CreatureObject(pBoss):getMaxHAM(i) * percent) then
                        return true
                    end
                end
            end
        end
    end
    return false
end

--[[
-- Old function. Possibly causing deadlocks
function combatHelper:logicBossState(pBoss, percent, stateString, checkState)
    if (pBoss == nil) then
		return false
    end

    if (readData(stateString) == checkState) then
        for i = 0, 6, 3 do
			if (CreatureObject(pBoss):getHAM(i) <= CreatureObject(pBoss):getMaxHAM(i) * percent) then
				return true
			end
        end
    end
    return false
end]]

function combatHelper:checkHAMCondition(pCreature, percent)
    local ret = false

    ObjectManager.withCreatureObject(pCreature, function(creature)
        if ((creature:getHAM(0) <= creature:getMaxHAM(0) * percent) or (creature:getHAM(3) <= creature:getMaxHAM(3) * percent) or (creature:getHAM(6) <= creature:getMaxHAM(6) * percent)) then
            ret = true
        end
    end)
    return ret
end

function combatHelper:setHAM(pCreature, percent)
    ObjectManager.withCreatureObject(pCreature, function(creature)
        creature:setHAM(0, creature:getMaxHAM(0) * percent)
        creature:setHAM(3, creature:getMaxHAM(3) * percent)
        creature:setHAM(6, creature:getMaxHAM(6) * percent)
    end)
    return 0
end

function combatHelper:awardGroupKill(pPlayer, x, z, y, maxDistance, screenPlayStateStr, screenPlayStateValue, msg, callback)
    -- initialze the callback with an empty call if there is none given
    callback = callback or function(groupMember) end
    ObjectManager.withCreatureObject(pPlayer, function(player)
        if (player:isGrouped()) then
            local groupSize = player:getGroupSize()
            for i = 0, groupSize -1 do
                local pGroupMember = player:getGroupMember(i)
          	    if (pGroupMember ~= nil)  then
                    if (SceneObject(pGroupMember):isPlayerCreature()) then
                        if self:checkCreatureWorldDistance(x, y, maxDistance, pGroupMember) == true then
                            ObjectManager.withCreatureObject(pGroupMember, function(groupMember)
                                groupMember:setScreenPlayState(screenPlayStateValue, screenPlayStateStr)
                                groupMember:sendSystemMessage(msg)
				callback(groupMember)
                            end)
                        end
                    end
		            end
	          end
        else
            player:setScreenPlayState(screenPlayStateValue, screenPlayStateStr)
            player:sendSystemMessage(msg)
	    callback(player)
        end
    end)
    return 0
end

function combatHelper:awardGroupKillwithCondCheck(pPlayer, x, z, y, maxDistance, screenPlayStateStr, screenPlayStateValue, msg, preCondSPStr, preCondSPval, callback)
    -- initialze the callback with an empty call if there is none given
    callback = callback or function(groupMember)
    	print('default callback')
    end
    ObjectManager.withCreatureObject(pPlayer, function(player)
        if (player:isGrouped()) then
	          local groupSize = player:getGroupSize()
            for i = 0, groupSize -1 do
                local pGroupMember = player:getGroupMember(i)
                if (pGroupMember ~= nil)  then
		                if (SceneObject(pGroupMember):isPlayerCreature()) then
		                    if self:checkCreatureDistance(x, z, y, maxDistance, pGroupMember) == true then
			                      ObjectManager.withCreatureObject(pGroupMember, function(groupMember)
                                if (groupMember:hasScreenPlayState(preCondSPval, preCondSPStr)) then
				                           groupMember:setScreenPlayState(screenPlayStateValue, screenPlayStateStr)
                                    groupMember:sendSystemMessage(msg)
				    callback(groupMember)
                                end
                            end)
                        end
                    end
                end
            end
        else
            if (player:hasScreenPlayState(preCondSPval, preCondSPStr)) then
                player:setScreenPlayState(screenPlayStateValue, screenPlayStateStr)
                player:sendSystemMessage(msg)
                callback(player)
            end
        end
    end)
    return 0
end

function combatHelper:addGroupBounty(pPlayer, x, z, y, maxDistance, reward, msg)
    ObjectManager.withCreatureObject(pPlayer, function(player)
        if (player:isGrouped()) then
            local groupSize = player:getGroupSize()
            for i = 0, groupSize -1 do
                local pGroupMember = player:getGroupMember(i)
                if (pGroupMember ~= nil)  then
                    if self:checkCreatureDistance(x, z, y, maxDistance, pGroupMember) == true then
                        if (SceneObject(pGroupMember):isPlayerCreature()) then
                            ObjectManager.withCreatureObject(pGroupMember, function(groupMember)
                                if (groupMember:hasScreenPlayState(1, "revelation_jedi_state")) then
                                    groupMember:addBounty(reward)
                                    groupMember:sendSystemMessage(msg)
                                end
                            end)
                        end
                    end
                end
            end
        else
            player:addBounty(reward)
            player:sendSystemMessage(msg)
        end
    end)
    return 0
end

function combatHelper:increaseFactionStanding(pPlayer, faction, value)
    ObjectManager.withCreatureObject(pPlayer, function(player)
        if (player:isGrouped()) then
            local groupSize = player:getGroupSize()
            for i = 0, groupSize -1 do
                local pGroupMember = player:getGroupMember(i)
                if (pGroupMember ~= nil)  then
                    if self:checkCreatureDistance(x, z, y, maxDistance, pGroupMember) == true then
                        if (SceneObject(pGroupMember):isPlayerCreature()) then
                            ObjectManager.withCreatureObject(pGroupMember, function(groupMember)
                                groupMember:increaseFactionStanding(faction,value)
                             end)
                        end
                    end
                end
            end
        else
            player:increaseFactionStanding(faction,value)
        end
    end)
    return 0
end

function combatHelper:getThreatHealMap(pMobile)
	if (pMobile == nil) then
		return
	end

	local attackerList = CreatureObject(pMobile):getDamageDealerList()

	if (attackerList == nil or #attackerList == 0) then
		return
	end

	local healerList = CreatureObject(pMobile):getHealingThreatList()

	if (healerList == nil) then
		return
	end

	if (#healerList ~= 0) then
		-- Combine tables
		for k,v in pairs(healerList) do
			table.insert(attackerList, v)
		end
		-- Only keep unique entries
		local hash = {}
		local res = {}
		for _,v in ipairs(attackerList) do
			if (not hash[v]) then
				res[#res+1] = v
				hash[v] = true
			end
		end
	end

	return attackerList
end

return combatHelper
