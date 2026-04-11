//
// Created by filip on 4/11/26.
//

#include "../include/MainController.hpp"

#include "engine/platform/PlatformController.hpp"

namespace app {
void MainController::initialize() {}
std::string_view MainController::name() const { return "app::MainController"; }
bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    return !platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down();
}
}// namespace app