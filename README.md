forgottenserver
===============

The Forgotten Server is a free and open-source MMORPG server emulator written in C++. It is a fork of the [OpenTibia Server](https://github.com/opentibia/server) project. To connect to the server, you can use [OTClient](https://github.com/edubart/otclient), [OpenTibiaUnity](https://github.com/slavidodo/OpenTibia-Unity) or [The Forgotten Client](https://github.com/SaiyansKing/The-Forgotten-Client)(work in progress).

### Features

- Attached Effect Support
- Shader Support

#### sample
```lua
item:setShader(<shader name>)
creature:setShader(<shader name>)
creature:attachEffectById(<effect id>, <temporary>(true | false)) -- Temporary = does not save in character
```

##### note: use storage to save a permanent effect on the player.
```lua
-- Sample
local EFFECT_STORAGE = 80000

player:setStorageValue(EFFECT_STORAGE, 7)
.
.
.
local effectId = player:getStorageValue(EFFECT_STORAGE)
if effectId ~= nil then
creature:attachEffectById(effectId)
end
```

#### WARNING: YOU MUST ENABLE THESE FEATURES IN THE CLIENT
```lua
g_game.enableFeature(Otc::GameItemShader);
g_game.enableFeature(Otc::GameCreatureShader);
g_game.enableFeature(Otc::GameCreatureAttachedEffect);
```

### Getting Started

* [Compiling](https://github.com/otland/forgottenserver/wiki/Compiling)
* [Scripting Reference](https://github.com/otland/forgottenserver/wiki/Script-Interface)

### Support

If you need help, please visit the [support forum on OTLand](https://otland.net/forums/support.16/). Our issue tracker is not a support forum, and using it as one will result in your issue being closed. If you were unable to get assistance in the support forum, you should consider [becoming a premium user on OTLand](https://otland.net/account/upgrades) which grants you access to the premium support forum and supports OTLand financially.

### Issues

We use the [issue tracker on GitHub](https://github.com/SaiyansKing/forgottenserver/issues). Keep in mind that everyone who is watching the repository gets notified by e-mail when there is activity, so be thoughtful and avoid writing comments that aren't meaningful for an issue (e.g. "+1"). If you'd like for an issue to be fixed faster, you should either fix it yourself and submit a pull request, or place a bounty on the issue.

### Repository

This repository isn't a simple fork from the main repository because it's conception is to deliver heavily optimization on the project by going to even breaking backward compatiblity if needed.

### Donation
If you like what I'm doing you can donate me :)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=jakubkubina@hotmail.com&rm=0&currency_code=USD)
