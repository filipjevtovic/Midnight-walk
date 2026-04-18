//
// Created by filip on 4/11/26.
//

#include <EnvironmentController.hpp>
#include <FlashlightController.hpp>
#include <LampController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {

void EnvironmentController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
}

std::string_view EnvironmentController::name() const {
    return "app::EnvironmentController";
}

void EnvironmentController::draw() {
    draw_well();
}

void EnvironmentController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void EnvironmentController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void EnvironmentController::draw_well() {
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    auto old_well = resource->model("old_well");
    auto main_shader = resource->shader("MainShader");
    main_shader->use();
    main_shader->set_mat4("projection", graphics->projection_matrix());
    main_shader->set_mat4("view", graphics->camera()->view_matrix());
    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    model = scale(model, glm::vec3(0.3f));
    model = rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    main_shader->set_mat4("model", model);

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader);

    auto lamps = engine::core::Controller::get<LampController>();
    lamps->set_point_lights(main_shader);

    old_well->draw(main_shader);
}

}// namespace app