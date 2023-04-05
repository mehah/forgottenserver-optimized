function onSay(player, words, param)
    local effect = tonumber(param)
    player:detachEffect(effect)

    return false
end
