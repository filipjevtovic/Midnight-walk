//
// Created by filip on 4/12/26.
//

#include <GUIController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

#include <imgui.h>

namespace app {

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
        platform->set_enable_cursor(is_enabled());
    }
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();
    ImGui::Begin("Current state");

    auto cam = graphics->camera();
    ImGui::Text("X: %f, Y: %f, Z: %f", cam->Position.x, cam->Position.y, cam->Position.z);

    ImGui::ColorEdit3("Flashlight color", m_selected_color);

    ImGui::End();
    graphics->end_gui();
}

std::string_view GUIController::name() const {
    return "app::GUIController";
}

glm::vec3 GUIController::color_selection() const {
    return glm::vec3(m_selected_color[0], m_selected_color[1], m_selected_color[2]);
}

}// namespace app