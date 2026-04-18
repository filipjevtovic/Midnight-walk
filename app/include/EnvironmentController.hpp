//
// Created by filip on 4/11/26.
//

#ifndef MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
#define MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
#include "engine/core/Controller.hpp"
#include "engine/resources/Model.hpp"

#define NUM_LAMPS (2)

namespace app {
class EnvironmentController : public engine::core::Controller {
public:
    std::string_view name() const override;

private:
    void draw() override;
    void initialize() override;
    void begin_draw() override;
    void end_draw() override;
    void draw_well();//placeholder model...
    void draw_street_lamps();
    void draw_lightbulbs();

    void set_point_lights(engine::resources::Shader *shader);
    std::vector<glm::vec3> lamp_positions{NUM_LAMPS};
    const glm::vec3 bulb_pos_offset{glm::vec3(0.0f, 7.5f, 0.0f)};
};
}// namespace app
#endif//MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
