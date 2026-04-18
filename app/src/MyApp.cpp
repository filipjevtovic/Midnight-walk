//
// Created by filip on 4/11/26.
//

#include "../include/MyApp.hpp"

#include "../include/EnvironmentController.hpp"
#include "../include/GUIController.hpp"
#include "../include/MainController.hpp"
#include "LampController.hpp"
#include "spdlog/spdlog.h"
#include <FlashlightController.hpp>

namespace app {

void MyApp::app_setup() {
    spdlog::info("App setup completed!");

    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto environment_controller = register_controller<app::EnvironmentController>();
    environment_controller->after(main_controller);

    auto gui_controller = register_controller<app::GUIController>();
    main_controller->before(gui_controller);

    auto flashlight_controller = register_controller<app::FlashlightController>();
    flashlight_controller->after(environment_controller);

    auto lamp_controller = register_controller<app::LampController>();
    lamp_controller->after(flashlight_controller);
}

}// namespace app