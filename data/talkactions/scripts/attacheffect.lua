function onSay(player, words, param)
    local effect = tonumber(param)
    player:attachEffectById(effect)

    return false
end
