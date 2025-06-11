#include <memory>

#include "engine.hh"
#include "editor.hh"
#include "texture_manager.hh"

namespace plane_quest::engine {

class Editor;

class EngineImpl {
    TextureManager texture_manager;

  public:
    EngineImpl() {}

    std::unique_ptr<Editor> create_editor(std::shared_ptr<Engine> engine) {
        return std::make_unique<Editor>(std::move(engine));
    }

    TextureManager &get_texture_manager() { return texture_manager; }

    const TextureManager &get_texture_manager() const {
        return texture_manager;
    }
};

Engine::Engine() : impl(std::make_unique<EngineImpl>()) {}

std::unique_ptr<Editor> Engine::new_editor() {
    return impl->create_editor(shared_from_this());
}

} // namespace plane_quest::engine