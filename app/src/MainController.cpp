//
// Created by filip on 4/11/26.
//

#include <MainController.hpp>

#include <GUIController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto cam = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        cam->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

    auto cam = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    cam->move_camera(engine::graphics::Camera::Movement::UP, 1);
    cam->move_camera(engine::graphics::Camera::Movement::BACKWARD, 3);
}

std::string_view MainController::name() const { return "app::MainController"; }

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    return !platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down();
}

void MainController::update() {
    update_camera();
}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto cam = graphics->camera();

    auto dt = platform->dt();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::FORWARD, dt * 2);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::LEFT, dt * 2);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt * 2);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::RIGHT, dt * 2);
    }
    // TODO: DELETE the following two in the final version (?)
    if (platform->key(engine::platform::KeyId::KEY_UP).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::UP, dt * 2);
    }
    if (platform->key(engine::platform::KeyId::KEY_DOWN).is_down()) {
        cam->move_camera(engine::graphics::Camera::Movement::DOWN, dt * 2);
    }
}

}// namespace app