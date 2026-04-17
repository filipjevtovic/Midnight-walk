//
// Created by filip on 4/11/26.
//

#include <MyApp.hpp>

#include <EnvironmentController.hpp>
#include <GUIController.hpp>
#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {

void MyApp::app_setup() {
    spdlog::info("App setup completed!");

    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto environment_controller = register_controller<app::EnvironmentController>();
    environment_controller->after(main_controller);

    auto gui_controller = register_controller<app::GUIController>();
    main_controller->before(gui_controller);
}

}// namespace app