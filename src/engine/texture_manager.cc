/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "texture_manager.hh"
#include "texture_key.hh"

namespace redseen::engine {

TextureManager::TextureManager() {}

TextureManager::SharedTexture TextureManager::get(const TextureKey &key) const {
    return texture_map.at(key);
}

bool TextureManager::remove(const TextureKey &key) {
    return texture_map.erase(key) == 1;
}

bool TextureManager::set(const TextureKey &key, const SharedTexture &txt,
                         bool replace) {
    return texture_map.emplace(std::make_pair(key, txt)).second;
}

} // namespace redseen::engine