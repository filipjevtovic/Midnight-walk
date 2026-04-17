//
// Created by filip on 17. 4. 2026..
//

#ifndef MATF_RG_PROJECT_FLASHLIGHTCONTROLLER_HPP
#define MATF_RG_PROJECT_FLASHLIGHTCONTROLLER_HPP

#include <engine/core/Engine.hpp>

namespace app {
class FlashlightController : public engine::core::Controller {
public:
    std::string_view name() const override;
    void setup_flashlight(engine::resources::Shader *shader);

private:
    void initialize() override;
};
}// namespace app

#endif//MATF_RG_PROJECT_FLASHLIGHTCONTROLLER_HPP
