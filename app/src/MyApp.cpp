//
// Created by filip on 4/11/26.
//

#include "../include/MyApp.hpp"

#include "../include/EnvironmentController.hpp"
#include "spdlog/spdlog.h"
#include <MainController.hpp>

namespace app {

void MyApp::app_setup() {
    spdlog::info("App setup completed!");

    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto environment_controller = register_controller<app::EnvironmentController>();
    environment_controller->after(main_controller);
}

}// namespace app