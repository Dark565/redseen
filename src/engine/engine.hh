#pragma once

#include <memory>

#include "common/noncopyable.hh"
#include "editor.hh"
#include "texture_manager.hh"

namespace plane_quest::engine {

class Editor;

class Engine : NonCopyable, public std::enable_shared_from_this<Engine> {
    std::unique_ptr<class EngineImpl> impl;

    Engine();

  public:
    static std::shared_ptr<Engine> create();

    std::unique_ptr<Editor> new_editor();
    TextureManager &get_texture_manager();
    const TextureManager &get_texture_manager() const;
};

} // namespace plane_quest::engine