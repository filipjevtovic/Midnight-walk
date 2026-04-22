//
// Created by filip on 18. 4. 2026.
//

#ifndef MATF_RG_PROJECT_LAMPCONTROLLER_HPP
#define MATF_RG_PROJECT_LAMPCONTROLLER_HPP

#include <engine/core/Engine.hpp>

#define NUM_LAMPS (6)

namespace app {
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

    void draw_lightbulbs() const;
    void draw_street_lamps() const;
};
}// namespace app

#endif//MATF_RG_PROJECT_LAMPCONTROLLER_HPP
