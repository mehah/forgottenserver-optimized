function onLogin(player)
    local loginStr = 'Welcome to ' .. configManager.getString(configKeys.SERVER_NAME) .. '!'
    if player:getLastLoginSaved() <= 0 then
        loginStr = loginStr .. ' Please choose your outfit.'
        player:sendOutfitWindow()
    else
        if loginStr ~= '' then
            player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
        end

        loginStr = string.format('Your last visit was on %s.', os.date('%a %b %d %X %Y', player:getLastLoginSaved()))
    end
    player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

    -- Stamina
    nextUseStaminaTime[player.uid] = 0

    -- Promotion
    local vocation = player:getVocation()
    local promotion = vocation:getPromotion()
    if player:isPremium() then
        local value = player:getStorageValue(PlayerStorageKeys.promotion)
        if not promotion and value ~= 1 then
            player:setStorageValue(STORAGEVALUE_PROMOTION, 1)
        elseif value == 1 then
            player:setVocation(promotion)
        end
    elseif not promotion then
        player:setVocation(vocation:getDemotion())
    end

    if player:getGroup():getAccess() and player:getAccountType() >= ACCOUNT_TYPE_GAMEMASTER then
        player:setShader("Outfit - Rainbow")
        player:attachEffectById(7)
        player:attachEffectById(8)
        player:attachEffectById(9, true) -- Temporary Effect
        player:setMapShader('Map - Heat', true)
    end

    -- Events
    player:registerEvent('PlayerDeath')
    player:registerEvent('DropLoot')
    return true
end
