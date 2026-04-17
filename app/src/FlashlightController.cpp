//
// Created by filip on 17. 4. 2026..
//

#include <FlashlightController.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {

void FlashlightController::initialize() {}
std::string_view FlashlightController::name() const { return "app::FlashlightController"; }
void FlashlightController::setup_flashlight(engine::resources::Shader *shader) {
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    auto camera_position = camera->Position;
    auto camera_direction = camera->Front;

    shader->set_vec3("flashlight.position", camera_position);
    shader->set_vec3("flashlight.direction", camera_direction);
    shader->set_vec3("flashlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader->set_vec3("flashlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->set_vec3("flashlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    // attentuation distance ~100:
    shader->set_float("flashlight.att_const", 1.0f);
    shader->set_float("flashlight.att_lin", 0.045f);
    shader->set_float("flashlight.att_quad", 0.0075f);

    shader->set_float("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("viewPos", camera_position);
}

}// namespace app