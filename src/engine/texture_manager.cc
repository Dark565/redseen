#include <memory>

#include "texture_key.hh"
#include "texture_manager.hh"

namespace plane_quest::engine {

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

} // namespace plane_quest::engine