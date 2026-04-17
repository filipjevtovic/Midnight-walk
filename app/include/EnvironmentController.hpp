//
// Created by filip on 4/11/26.
//

#ifndef MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
#define MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
class EnvironmentController : public engine::core::Controller {
public:
    std::string_view name() const override;

private:
    void draw() override;
    void initialize() override;
    void begin_draw() override;
    void end_draw() override;
    void draw_tree();//placeholder model...
};
}// namespace app
#endif//MATF_RG_PROJECT_ENVIRONMENTCONTROLLER_HPP
