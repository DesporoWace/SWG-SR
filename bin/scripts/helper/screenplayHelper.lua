-- Author: Matthias Muente (matthias.muente@gmx.de)
-- https://revelationonline.net

-- Thanks HK!

local screenplayHelper = {}
local ObjectManager = require("managers.object.object_manager")

function screenplayHelper:spawnMobileAndIdle(planet, mobile, spawntimer, x, z, y, rot, cell)
	local pMobile = spawnMobile(planet, mobile, spawntimer, x, z, y, rot, cell)
	local aiAgentSquad = LuaAiAgent(pMobile)
	if (aiAgentSquad ~= nil) then
	 	aiAgentSquad:setAiTemplate("idlewander_attacking")
	end
	return pMobile
end

---- build function
function screenplayHelper:spawnHorizontalSceneObjects(planet, startX, startY, iffName, count, stepDiff)
	for i = 0 , count - 1 , 1 do
		spawnSceneObject(planet, iffName, startX - (i*stepDiff), 9999, startY, 0, 0, 0, 0, 0)
	end
end

function screenplayHelper:spawnVerticalSceneObjects(planet, startX, startY, iffName, count, stepDiff)
	for i = 0 , count - 1 , 1 do
		spawnSceneObject(planet, iffName, startX , 9999, startY - (i*stepDiff), 0, 0.707, 0, -0.707, 0)
	end
end

function screenplayHelper:spawnDiagonalSceneObjects(planet, startX, startY, iffName, count, stepDiff, invert, dxr, dyr)
	for i = 0 , count - 1 , 1 do
		spawnSceneObject(planet, iffName, startX - (i*stepDiff*0.707*invert) , 9999, startY - (i*stepDiff*0.707*invert), 0, dxr, 0, dyr, 0)
	end
end

function screenplayHelper:spawnImpTurret(planet, iff, x, z, y, cell, dw, dx, dy, dz, pvpmask)
	local turret = spawnSceneObject(planet,iff, x, z, y, cell, dw, dx, dy, dz)
	local turretObj = LuaTangibleObject(turret)
	turretObj:setFaction(3679112276)
	turretObj:setPvpStatusBitmask(pvpmask)
	return turret
end


-- directions are N,NE,E,SE,S,SW,W,NW
function screenplayHelper:spawnSceneObjectlineToDirection(planet, iff, x, y, direction, count, objlen)
	xpos = 0
	ypos = 0
	dw = 0
	dz = 0
	len = objlen/2
	for i = 0 , count - 1 ,1 do
		if (direction == "N") then
			xpos = x
			ypos = y + (len *(1+i*2))
			dw = 0.7071 --0
			dz = -0.70710 --0
		elseif (direction == "NE") then
			xpos = x + (0.707 * len *(1+i*2))
			ypos = y + (0.707 * len *(1+i*2))
			dw = 0.9238
			dz = -0.3826
		elseif (direction == "E") then
			xpos = x + (len *(1+i*2))
			ypos = y
			dw = 0--0.7071
			dz = 0---0.7071
		elseif (direction == "SE") then
			xpos = x + (0.707 * len *(1+i*2))
			ypos = y - (0.707 * len *(1+i*2))
			dw = 0.3826
			dz = -0.9238
		elseif (direction == "S") then
			xpos = x
			ypos = y - (len *(1+i*2))
			dw = 0.7071 --0
			dz = -0.7071 ---1
		elseif (direction == "SW") then
			xpos = x - (0.707 * len *(1+i*2))
			ypos = y - (0.707 * len *(1+i*2))
			dw = 0.3826
			dz = 0.9238
		elseif (direction == "W") then
			xpos = x - (len *(1+i*2))
			ypos = y
			dw = 0--0.7071
			dz = 0---0.7071
		elseif (direction == "NW") then
			xpos = x - (0.707 * len *(1+i*2))
			ypos = y + (0.707 * len *(1+i*2))
			dw = 0.9238
			dz = 0.3826
		end
		spawnSceneObject(planet,iff, xpos, 9999, ypos, 0, dw, 0, dz, 0)
	end
end


--behavior = idlewander, patrol, wait, follow
function screenplayHelper:setBehavior(squadid, behavior, count)

	for i = 0 , count - 1 , 1 do
		local stringhelper = (squadid..i)
		local pSquad = getCreatureObject(readData(stringhelper..":objectid"))
		local aiAgent_Squad = LuaAiAgent(pSquad)

		if (aiAgent_Squad ~= nil) then
			aiAgent_Squad:setAiTemplate(behavior)
		end
	end
end

--destroy Objects from world and DB
function screenplayHelper:destroy(objectID, deleteFromDB)
	local sObj = getSceneObject(objectID)
	if (sObj ~= nil) then
		local oObj = LuaSceneObject(sObj)
		if (oObj ~= nil) then
			if (deleteFromDB == true) then
				--oObj:destroyObjectFromDatabase()
			end
			oObj:destroyObjectFromWorld()
		end
	end
end

--doorhelper to set door permissions
function screenplayHelper:setupPermissionGroups(KeypadStringID, lockedCells)
	for i = 1, #lockedCells, 1 do
		local pCell = getSceneObject(lockedCells[i])
		if pCell ~= nil then
			ObjectManager.withSceneObject(pCell, function(cell)
				cell:setContainerInheritPermissionsFromParent(false)
				cell:clearContainerDefaultDenyPermission(WALKIN)
				cell:clearContainerDefaultAllowPermission(WALKIN)
				cell:setContainerAllowPermission(KeypadStringID .. i, WALKIN)
				cell:setContainerDenyPermission(KeypadStringID .. i, MOVEIN)
			end)
		end
	end
end

function screenplayHelper:givePermission(pPlayer, permissionGroup)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(ghost)
		ghost:addPermissionGroup(permissionGroup, true)
	end)
end

function screenplayHelper:removePermission(pPlayer, permissionGroup)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(ghost)
		if (ghost:hasPermissionGroup(permissionGroup)) then
			ghost:removePermissionGroup(permissionGroup, true)
		end
	end)
end

function screenplayHelper:hasPermission(pPlayer, permissionGroup)
	ObjectManager.withCreaturePlayerObject(pPlayer, function(ghost)
		return ghost:hasPermissionGroup(permissionGroup)
	end)
end


function screenplayHelper:searchContainerForObject(pContainer, ObjectName, deleteFlag, pPlayer)
    local itemCounter = 0
    ObjectManager.withSceneObject(pContainer, function(container)
        local containerSize = container:getContainerObjectsSize()
        local i = 0
        for i = 0, containerSize - 1, 1 do
            local pInvObj = container:getContainerObject(i)
            ObjectManager.withSceneObject(pInvObj, function(invObj)
                -- Get Objectnames from Each Item and check if its the choosen Item in this case a generic tool :P
                if (invObj:getObjectName() == ObjectName)   then
                    itemCounter = itemCounter + 1
                    --if (deleteFlag == true) then
                    --invObj:destroyObjectFromWorld()
                    --invObj:destroyObjectFromDatabase()
                    --end
                else
                    -- check if object is a container
                    if (invObj:getContainerObjectsSize() > 0) then
                    	itemCounter = itemCounter + screenplayHelper:searchContainerForObject(pInvObj, ObjectName, deleteFlag, pPlayer)
                    end
                end
            end)
        end
    end)
    return itemCounter
end

--slottetObject = "inventory" , "back"
function screenplayHelper:searchInventoryForObject(pPlayer, ObjectName, deleteFlag, slottetObject)
local itemCounter = 0
	ObjectManager.withCreatureObject(pPlayer, function(player)
		local pInventory = player:getSlottedObject(slottetObject)
		itemCounter = screenplayHelper:searchContainerForObject(pInventory, ObjectName, deleteFlag, pPlayer)
	end)
return itemCounter
end

function screenplayHelper:buyPlayerHABuff(conversingPlayer, strength, duration, credits)
	local ret = false
	if (self:removeCash(conversingPlayer, credits)) then
		self:doEnhanceCharacterHealthAction(conversingPlayer, strength, duration)
		ret = true
	end
	return ret
end

function screenplayHelper:buyPlayerBuffRemove(conversingPlayer, credits)
  local ret = false
  if (self:removeCash(conversingPlayer, credits)) then
    self:doRemoveBuffsFromPlayer(conversingPlayer)
    ret = true
  end
  return ret
end

function screenplayHelper:buyPlayerHeal(conversingPlayer, credits)
  local ret = false
  if (self:removeCash(conversingPlayer, credits)) then
    self:doHealPlayer(conversingPlayer)
    ret = true
  end
  return ret
end

function screenplayHelper:doEnhanceCharacterHealthAction(conversingPlayer, strength, duration)
	local creature = LuaCreatureObject(conversingPlayer)
	local ret = false
	if ( creature ~= nil ) then
		creature:doEnhanceCharacterHealthAction(strength, duration)
		creature:sendSystemMessage("A painful needle bite like a snake in your arm !")
	end
end

function screenplayHelper:buyPlayerMBuff(conversingPlayer, strength, duration, credits)
	local ret = false
	if (self:removeCash(conversingPlayer, credits)) then
		self:doEnhanceCharacterMind(conversingPlayer, strength, duration)
		ret = true
	end
	return ret
end

function screenplayHelper:doEnhanceCharacterMind(conversingPlayer, strength, duration)
	local creature = LuaCreatureObject(conversingPlayer)
	local ret = false
	if ( creature ~= nil ) then
		creature:doEnhanceCharacterMind(strength, duration)
		creature:sendSystemMessage("You feel refreshed")
	end
end

function screenplayHelper:doHealPlayer(conversingPlayer)
  local creature = LuaCreatureObject(conversingPlayer)
  local ret = false
  if ( creature ~= nil ) then
    creature:doHealPlayer()
    creature:sendSystemMessage("Ahhhhh Bacta !")
  end
end

function screenplayHelper:doRemoveBuffsFromPlayer(conversingPlayer)
  local creature = LuaCreatureObject(conversingPlayer)
  local ret = false
  if ( creature ~= nil ) then
    creature:doRemoveBuffsFromPlayer()
    creature:sendSystemMessage("the Buffs are fading away")
  end
end

function screenplayHelper:removeCash(conversingPlayer, credits)
	local creature = LuaCreatureObject(conversingPlayer)
	local ret = false
	if ( creature ~= nil ) then
		local playercredits = creature:getCashCredits()
		if (playercredits >= credits) then
			creature:subtractCashCredits(credits)
			creature:sendSystemMessage("You pay " .. credits .. " credits")
			ret = true
		else
			creature:sendSystemMessage("You do not have enough credits.")
		end
	end
	return ret
end

function screenplayHelper:despawnCreature(pMobile)
    ObjectManager.withCreatureObject(pMobile, function(mobile)
        self:destroy(mobile:getObjectID(),true)
    end)
end

function screenplayHelper:decreaseFactionStanding(conversingPlayer, faction, value)
        local creature = LuaCreatureObject(conversingPlayer)
        local pPlayer = creature:getPlayerObject()
        local playerObj = LuaPlayerObject(pPlayer)
        if playerObj:getFactionStanding(faction) < 20 + value then
            return false
        else
            playerObj:decreaseFactionStanding(faction ,value)
            return true
        end
        return false
end

return screenplayHelper
