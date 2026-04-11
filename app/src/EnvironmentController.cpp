//
// Created by filip on 4/11/26.
//

#include "EnvironmentController.hpp"

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"

namespace app {

void EnvironmentController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
}

std::string_view EnvironmentController::name() const {
    return "app::EnvironmentController";
}

void EnvironmentController::draw() {
    draw_tree();
}

void EnvironmentController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void EnvironmentController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void EnvironmentController::draw_tree() {
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    auto old_well = resource->model("old_well");
    auto basic_shader = resource->shader("basic");
    basic_shader->use();
    basic_shader->set_mat4("projection", graphics->projection_matrix());
    basic_shader->set_mat4("view", graphics->camera()->view_matrix());
    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    model = scale(model, glm::vec3(0.3f));
    model = rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    basic_shader->set_mat4("model", model);
    old_well->draw(basic_shader);
}

}// namespace app