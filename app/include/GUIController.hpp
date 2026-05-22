//
// Created by filip on 4/12/26.
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/vec3.hpp>

namespace app {

class GUIController : public engine::core::Controller {
public:
    std::string_view name() const override;
    glm::vec3 color_selection() const;

private:
    void initialize() override;
    void poll_events() override;
    void draw() override;

    float m_selected_color[3] = {1.0f, 1.0f, 1.0f};
};

}// namespace app

#endif//MATF_RG_PROJECT_GUICONTROLLER_HPP
