/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 * Copyright (C) 2019-2021  Saiyans King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "script.h"
#include "configmanager.h"

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

extern LuaEnvironment g_luaEnvironment;
extern ConfigManager g_config;

Scripts::Scripts() :
    scriptInterface("Scripts Interface")
{
    scriptInterface.initState();
}

Scripts::~Scripts()
{
    scriptInterface.reInitState();
}

bool Scripts::loadScripts(const std::string& folderName, const bool isLib, const bool reload)
{
    const auto dir = fs::current_path() / "data" / folderName;
    if (!exists(dir) || !is_directory(dir)) {
        std::cout << "[Warning - Scripts::loadScripts] Can not load folder '" << folderName << "'." << std::endl;
        return false;
    }

    const fs::recursive_directory_iterator endit;
    std::vector<fs::path> v;
    const std::string disable = "#";
    for (fs::recursive_directory_iterator it(dir); it != endit; ++it) {
        auto fn = it->path().parent_path().filename();
        if ((fn == "lib" && !isLib) || fn == "events") {
            continue;
        }
        if (is_regular_file(*it) && it->path().extension() == ".lua") {
            const size_t found = it->path().filename().string().find(disable);
            if (found != std::string::npos) {
                if (g_config.getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
                    std::cout << "> " << it->path().filename().string() << " [disabled]" << std::endl;
                }
                continue;
            }
            v.push_back(it->path());
        }
    }
    sort(v.begin(), v.end());
    std::string redir;
    for (auto& it : v)
    {
        const std::string scriptFile = it.string();
        if (!isLib) {
            if (redir.empty() || redir != it.parent_path().string()) {
                auto p = fs::path(it.relative_path());
                if (g_config.getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
                    std::cout << ">> [" << p.parent_path().filename() << "]" << std::endl;
                }
                redir = it.parent_path().string();
            }
        }

        if (scriptInterface.loadFile(scriptFile) == -1) {
            std::cout << "> " << it.filename().string() << " [error]" << std::endl;
            std::cout << "^ " << scriptInterface.getLastLuaError() << std::endl;
            continue;
        }

        if (g_config.getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
            if (!reload) {
                std::cout << "> " << it.filename().string() << " [loaded]" << std::endl;
            } else {
                std::cout << "> " << it.filename().string() << " [reloaded]" << std::endl;
            }
        }
    }

    return true;
}
