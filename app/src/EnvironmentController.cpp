//
// Created by filip on 4/11/26.
//

#include "EnvironmentController.hpp"

#include "../../engine/libs/glad/include/glad/glad.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include <FlashlightController.hpp>

#include <vector>

namespace app {

void EnvironmentController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
}

std::string_view EnvironmentController::name() const {
    return "app::EnvironmentController";
}

void EnvironmentController::draw() {
    draw_well();
    draw_lightbulbs();
    draw_street_lamps();
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

    old_well->draw(main_shader);
}

void EnvironmentController::draw_street_lamps() {
    // Model taken from: https://sketchfab.com/3d-models/old-english-street-lamp-obj-6341be23fc704d19967944359a410d68
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    std::vector<engine::resources::Model *> lamps(NUM_LAMPS);
    for (size_t i = 0; i < NUM_LAMPS; i++) {
        lamps[i] = resource->model("street_lamp");
    }
    auto main_shader = resource->shader("MainShader");
    main_shader->use();
    main_shader->set_mat4("projection", graphics->projection_matrix());
    main_shader->set_mat4("view", graphics->camera()->view_matrix());

    std::vector<glm::mat4> models(NUM_LAMPS, glm::mat4(1.0f));

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader);

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        models[i] = glm::translate(models[i], glm::vec3(0.0f, 0.0f, -7.0f));
        main_shader->set_mat4("model", models[i]);
        lamps[i]->draw(main_shader);
    }
}

void EnvironmentController::draw_lightbulbs() {
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    std::vector<engine::resources::Model *> bulbs(NUM_LAMPS);
    for (size_t i = 0; i < NUM_LAMPS; i++) {
        bulbs[i] = resource->model("lightbulb");
    }
    auto basic_shader = resource->shader("basic");
    basic_shader->use();
    basic_shader->set_mat4("projection", graphics->projection_matrix());
    basic_shader->set_mat4("view", graphics->camera()->view_matrix());

    std::vector<glm::mat4> models(NUM_LAMPS, glm::mat4(1.0f));

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        models[i] = glm::translate(models[i], glm::vec3(0.0f, 7.5f, -7.0f));
        models[i] = glm::scale(models[i], glm::vec3(0.3f, 0.3f, 0.3f));
        basic_shader->set_mat4("model", models[i]);
        bulbs[i]->draw(basic_shader);
    }
}

}// namespace app