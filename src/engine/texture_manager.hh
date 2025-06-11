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