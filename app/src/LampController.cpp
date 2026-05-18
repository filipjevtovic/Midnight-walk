//
// Created by filip on 18. 4. 2026.
//

#include <FlashlightController.hpp>
#include <LampController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {

size_t LampController::num_lamps() const {
    return NUM_LAMPS;
}

extern engine::resources::Bloom *bloom;
extern engine::resources::Shader *blur_shader, *final_shader;

void LampController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    //lamp_positions[0] = glm::vec3(0.0f, 0.0f, -7.0f);
    for (int i = 0; i < NUM_LAMPS / 2; i++) {
        lamp_positions[i] = glm::vec3(13.0f, 0.0f, -16.0f + i * 12.0f);
    }
    for (int i = 0; i < NUM_LAMPS / 2; i++) {
        lamp_positions[i + NUM_LAMPS / 2] = glm::vec3(-13.0f, 0.0f, -10.0f + i * 12.0f);
    }
}

std::string_view LampController::name() const {
    return "app::LampController";
}

void LampController::draw() {
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    draw_lightbulbs();
    draw_street_lamps();

    graphics->bloom_blur(blur_shader, bloom);
    graphics->bloom_draw(final_shader, bloom);
}

void LampController::set_point_lights(engine::resources::Shader *shader) const {
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    auto camera_position = camera->Position;

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto frame_time = platform->frame_time();
    shader->set_float("time", frame_time.current);

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        std::string id = "lamps[" + std::to_string(i) + "]";

        shader->set_vec3(id + ".position", lamp_positions[i] + bulb_pos_offset);

        // attentuation distance ~160
        shader->set_float(id + ".att_const", 1.0f);
        shader->set_float(id + ".att_lin", 0.027f);
        shader->set_float(id + ".att_quad", 0.0028f);

        shader->set_vec3(id + ".ambient", glm::vec3(0.05f, 0.05f, 0.05f) * lighbulb_color);
        shader->set_vec3(id + ".diffuse", glm::vec3(0.8f, 0.8f, 0.8f) * lighbulb_color);
        shader->set_vec3(id + ".specular", glm::vec3(1.0f, 1.0f, 1.0f) * lighbulb_color);

        switch (m_flicker_states[i]) {
            case FlickerState::NONE:
            case FlickerState::START:
            case FlickerState::PAUSE: {
                // 1 * cos(0 * t + 0) = 1
                shader->set_float(id + ".flicker_a", 1.0f);
                shader->set_float(id + ".flicker_b", 0.0f);
                shader->set_float(id + ".flicker_c", 0.0f);
                break;
            }
            case FlickerState::FLICKER1: {
                shader->set_float(id + ".flicker_a", 0.95f);
                shader->set_float(id + ".flicker_b", 101.0f);
                shader->set_float(id + ".flicker_c", 1001.0f);
                break;
            }
            case FlickerState::FLICKER2: {
                shader->set_float(id + ".flicker_a", 0.75f);
                shader->set_float(id + ".flicker_b", 2345.0f);
                shader->set_float(id + ".flicker_c", 8765.0f);
                break;
            }
            case FlickerState::DEAD: {
                // 0 * ... = 0
                shader->set_float(id + ".flicker_a", 0.0f);
                shader->set_float(id + ".flicker_b", 1.0f);
                shader->set_float(id + ".flicker_c", 0.0f);
                break;
            }
            default:
                std::unreachable();
        }
    }

    shader->set_vec3("viewPos", camera_position);
}

void LampController::draw_street_lamps() const {
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

    main_shader->set_vec2("tiling", glm::vec2(1.0f, 1.0f));

    std::vector<glm::mat4> models(NUM_LAMPS, glm::mat4(1.0f));

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader);

    set_point_lights(main_shader);

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        models[i] = glm::translate(models[i], lamp_positions[i]);
        main_shader->set_mat4("model", models[i]);
        lamps[i]->draw(main_shader);
    }
}

void LampController::draw_lightbulbs() const {
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

    basic_shader->set_vec3("lightColor", lighbulb_color);

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    basic_shader->set_float("time", platform->frame_time().current);

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        glm::vec3 bulb_pos = lamp_positions[i] + bulb_pos_offset;
        models[i] = glm::translate(models[i], bulb_pos);
        models[i] = glm::scale(models[i], glm::vec3(0.3f, 0.3f, 0.3f));
        basic_shader->set_mat4("model", models[i]);

        switch (m_flicker_states[i]) {
            case FlickerState::NONE:
            case FlickerState::START:
            case FlickerState::PAUSE: {
                // 1 * cos(0 * t + 0) = 1
                basic_shader->set_float("flicker_a", 1.0f);
                basic_shader->set_float("flicker_b", 0.0f);
                basic_shader->set_float("flicker_c", 0.0f);
                break;
            }
            case FlickerState::FLICKER1: {
                basic_shader->set_float("flicker_a", 0.95f);
                basic_shader->set_float("flicker_b", 101.0f);
                basic_shader->set_float("flicker_c", 1001.0f);
                break;
            }
            case FlickerState::FLICKER2: {
                basic_shader->set_float("flicker_a", 0.75f);
                basic_shader->set_float("flicker_b", 2345.0f);
                basic_shader->set_float("flicker_c", 8765.0f);
                break;
            }
            case FlickerState::DEAD: {
                // 0 * ... = 0
                basic_shader->set_float("flicker_a", 0.0f);
                basic_shader->set_float("flicker_b", 1.0f);
                basic_shader->set_float("flicker_c", 0.0f);
                break;
            }
            default:
                std::unreachable();
        }

        bulbs[i]->draw(basic_shader);
    }
}

void LampController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    for (size_t i = 0; i < NUM_LAMPS; i++) {
        auto elapsed = platform->frame_time().current - m_last_change[i];
        if (m_flicker_states[i] == FlickerState::START && elapsed >= 1.5) {
            m_flicker_states[i] = FlickerState::FLICKER1;
            m_last_change[i] = platform->frame_time().current;
        } else if (m_flicker_states[i] == FlickerState::FLICKER1 && elapsed >= 2.0) {
            m_flicker_states[i] = FlickerState::PAUSE;
            m_last_change[i] = platform->frame_time().current;
        } else if (m_flicker_states[i] == FlickerState::PAUSE && elapsed >= 2.0) {
            m_flicker_states[i] = FlickerState::FLICKER2;
            m_last_change[i] = platform->frame_time().current;
        } else if (m_flicker_states[i] == FlickerState::FLICKER2 && elapsed >= 2.0) {
            m_flicker_states[i] = FlickerState::DEAD;
            m_last_change[i] = platform->frame_time().current;
        }
    }
}

void LampController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F).state() == engine::platform::Key::State::JustPressed) {
        m_last_change[0] = platform->frame_time().current;
        m_flicker_states[0] = FlickerState::START;
        m_last_change[4] = platform->frame_time().current + 0.2f;
        m_flicker_states[4] = FlickerState::START;
    }
}

}// namespace app