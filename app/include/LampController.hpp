//
// Created by filip on 18. 4. 2026.
//

#ifndef MATF_RG_PROJECT_LAMPCONTROLLER_HPP
#define MATF_RG_PROJECT_LAMPCONTROLLER_HPP

#include <engine/graphics/Bloom.hpp>
#include <engine/core/Engine.hpp>

#define NUM_LAMPS (6)

namespace app {

enum class FlickerState {
    NONE = 0,
    START,
    FLICKER1,
    PAUSE,
    FLICKER2,
    DEAD
};

class LampController : public engine::core::Controller {
public:
    std::string_view name() const override;

    size_t num_lamps() const;
    void set_point_lights(engine::resources::Shader *shader) const;
    std::vector<glm::vec3> lamp_positions{NUM_LAMPS};
    const glm::vec3 bulb_pos_offset{glm::vec3(0.0f, 7.5f, 0.0f)};
    const glm::vec3 lighbulb_color{glm::vec3(0.9f, 0.75f, 0.55f)};

private:
    void initialize() override;
    void draw() override;
    void update() override;
    void draw_lightbulbs() const;
    void draw_street_lamps() const;
    void poll_events() override;

    std::vector<float> m_last_change{};
    std::vector<app::FlickerState> m_flicker_states{NUM_LAMPS, static_cast<app::FlickerState>(0)};

    engine::graphics::Bloom *m_bloom;
    engine::resources::Shader *m_blur_shader;
    engine::resources::Shader *m_final_shader;
};
}// namespace app

#endif//MATF_RG_PROJECT_LAMPCONTROLLER_HPP
