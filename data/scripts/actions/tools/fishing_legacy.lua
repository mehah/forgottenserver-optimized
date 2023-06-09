if (CLIENT_VERSION >= 800) then
    return
end

local waterIds = {493, 4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616, 4617, 4618, 4619, 4620, 4621, 4622, 4623,
                  4624, 4625}
local useWorms = true

local fishing = Action()

function fishing.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local targetId = target.itemid
    if not table.contains(waterIds, target.itemid) then
        return false
    end

    toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
    if targetId == 493 then
        return true
    end

    player:addSkillTries(SKILL_FISHING, 1)
    if math.random(1, 100) <=
        math.min(math.max(10 + (player:getEffectiveSkillLevel(SKILL_FISHING) - 10) * 0.597, 10), 50) then
        if useWorms and not player:removeItem(3976, 1) then
            return true
        end

        player:addItem(2267, 1)
    end
    return true
end

fishing:id(2580)
fishing:allowFarUse(true)
fishing:register()
