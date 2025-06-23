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

#pragma once

#include <unordered_map>

#include "texture_key.hh"

namespace plane_quest::render {
class Texture;
}
namespace plane_quest::engine {

class TextureManager {
    using SharedTexture = std::shared_ptr<const render::Texture>;
    std::unordered_map<TextureKey, SharedTexture> texture_map;

  public:
    TextureManager();

    /** Get a texture assigned to a given name
    Throws std::out_of_range if texture is not found. */
    SharedTexture get(const TextureKey &) const;

    /** Remove a texture assigned to a given name.
    Returns true if texture was found and removed. */
    bool remove(const TextureKey &);

    /** Assigns texture for a given key.
    Returns true if texture was successfully assigned or
    false if replace is false and a texture with the given
    name already exists. */
    bool set(const TextureKey &, const SharedTexture &, bool replace = false);
};
} // namespace plane_quest::engine