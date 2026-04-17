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

    auto campos = graphics->camera()->Position;
    auto camdir = graphics->camera()->Front;
    main_shader->set_vec3("flashlight.position", campos);
    main_shader->set_vec3("flashlight.direction", camdir);
    main_shader->set_vec3("flashlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    main_shader->set_vec3("flashlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    main_shader->set_vec3("flashlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    // attentuation distance ~65:
    main_shader->set_float("flashlight.att_const", 1.0f);
    main_shader->set_float("flashlight.att_lin", 0.07f);
    main_shader->set_float("flashlight.att_quad", 0.017f);

    main_shader->set_float("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
    main_shader->set_float("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    main_shader->set_vec3("viewPos", campos);
    old_well->draw(main_shader);
}

}// namespace app