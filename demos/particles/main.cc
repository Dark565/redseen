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

#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <random>

#include "engine/engine.hh"
#include "engine/camera.hh"
#include "engine/object/basic_object.hh"
#include "engine/object/object.hh"
#include "render/model.hh"
#include "render/opengl_mesh_handle.hh"
#include "ui/window.hh"
#include "engine/event_observer.hh"
#include "engine/renderers/opengl_renderer.hh"
#include "engine/mesh_factories/sphere_mesh_factory.hh"
#include "engine/model/opengl_model.hh"
#include "ui/window_event.hh"
#include "config.hh"
#include "bullet.hh"

namespace redseen::demos::particles {

class TestWindowObserver : public engine::EventObserver {
  public:
    TestWindowObserver(std::shared_ptr<engine::Engine> engine,
                       std::shared_ptr<engine::Model> bullet_model)
        : engine(engine), bullet_model_shared(bullet_model) {
        rand_device = std::make_unique<std::random_device>();
        mt_gen = std::make_unique<std::mt19937>((*rand_device)());
        vel_dist = std::make_unique<std::uniform_real_distribution<float>>(
            -1.0f, 1.0f);
    }

    engine::ObserverReturnSignal on_event(const engine::Event &event) override {
        if (event.has_name(ui::window_event::KEY)) {
            handleKeyEvent(static_cast<const ui::window_event::Key &>(event));
        }

        return engine::ObserverReturnSignal::CONTINUE;
    }

  private:
    void handleKeyEvent(const ui::window_event::Key &event) {
        const float cameraSpeed = 0.1f;
        const float rotationSpeed = 2.0f;

        auto &camera = engine->get_player_camera();

        if (event.action == ui::window_event::Action::PRESS) {
            switch (event.key) {
            case GLFW_KEY_W:
                camera.move(camera.getFront() * cameraSpeed);
                break;
            case GLFW_KEY_S:
                camera.move(-camera.getFront() * cameraSpeed);
                break;
            case GLFW_KEY_A:
                camera.move(-camera.getRight() * cameraSpeed);
                break;
            case GLFW_KEY_D:
                camera.move(camera.getRight() * cameraSpeed);
                break;
            case GLFW_KEY_LEFT:
                camera.rotate(-rotationSpeed, 0.0f);
                break;
            case GLFW_KEY_RIGHT:
                camera.rotate(rotationSpeed, 0.0f);
                break;
            case GLFW_KEY_UP:
                camera.rotate(0.0f, rotationSpeed);
                break;
            case GLFW_KEY_DOWN:
                camera.rotate(0.0f, -rotationSpeed);
                break;
            case GLFW_KEY_Q:
                exit(0);
                break;
            case GLFW_KEY_C:
                createBullet();
                break;
            }
        }
    }

    void createBullet() {
        auto &camera = engine->get_player_camera();
        glm::vec3 camera_pos = camera.getPosition();
        glm::vec3 camera_front = camera.getFront();

        const float spawn_distance = 0.8f;
        const float base_bullet_speed = 1e-2f;
        const float random_vel_spread = 1e-2f;

        glm::vec3 bullet_spawn_pos = camera_pos + camera_front * spawn_distance;

        glm::vec3 random_offset_vel =
            glm::vec3((*vel_dist)(*mt_gen) * random_vel_spread,
                      (*vel_dist)(*mt_gen) * random_vel_spread,
                      (*vel_dist)(*mt_gen) * random_vel_spread);

        glm::vec3 bullet_velocity =
            camera_front * base_bullet_speed + random_offset_vel;

        static int bullet_counter = 0;
        std::string bullet_name = "bullet_" + std::to_string(bullet_counter++);

        auto new_bullet_object =
            engine->get_object_manager()->create_object<Bullet>(
                bullet_name, bullet_spawn_pos, this->bullet_model_shared,
                bullet_velocity, 0.0f);
    }

    std::shared_ptr<engine::Engine> engine;
    std::shared_ptr<engine::Model> bullet_model_shared;

    std::unique_ptr<std::random_device> rand_device;
    std::unique_ptr<std::mt19937> mt_gen;
    std::unique_ptr<std::uniform_real_distribution<float>> vel_dist;
};

} // namespace redseen::demos::particles

int main() {
    using namespace redseen;

    auto engine = engine::Engine::create();

    ui::WindowConfig config;
    config.width = 1920;
    config.height = 1080;
    config.name = "Engine Test Window";

    auto window = std::make_shared<ui::Window>(config);
    window->show();

    auto renderer = std::make_shared<engine::renderers::OpenGLRenderer>(
        engine, window->getDrawer());

    engine->set_renderer(renderer);

    auto &camera = engine->get_player_camera();

    camera = engine::Camera(glm::vec3(0.0f, 0.0f, 3.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

    auto bullet_mesh =
        std::shared_ptr(engine::mesh_factories::Sphere(0.05f).create_mesh());
    auto mesh_handle = std::shared_ptr(
        render::OpenGLMeshHandle::create_from_mesh(*bullet_mesh));

    auto bullet_ll_model =
        std::make_shared<render::Model>(mesh_handle, nullptr);
    bullet_ll_model->setColor({1.0, 0.0, 0.0});

    auto bullet_model =
        std::make_shared<engine::model::OpenGLModel>(bullet_ll_model);

    auto observer = std::make_shared<demos::particles::TestWindowObserver>(
        engine, bullet_model);

    engine->get_event_dispatcher()->register_observer(
        "test_ui", {ui::window_event::KEY, ui::window_event::MOUSE_MOVE},
        demos::particles::PRIORITY_CLASS, 0, observer);

    engine->get_event_producer_container()->add_producer("window", window);

    engine->run();
    window->hide();
    return 0;
}
