//
// Created by filip on 4/11/26.
//

#include "../include/MainController.hpp"

#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto cam = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    cam->rotate_camera(position.dx, position.dy);
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
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
}

}// namespace app