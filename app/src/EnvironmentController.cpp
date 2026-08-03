//
// Created by filip on 4/11/26.
//

#include <EnvironmentController.hpp>
#include <FlashlightController.hpp>
#include <GUIController.hpp>
#include <LampController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {

void EnvironmentController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    const auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    const auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    m_bloom = graphics->bloom();
    m_blur_shader = resources->shader("BlurShader");
    m_final_shader = resources->shader("FinalShader");
}

std::string_view EnvironmentController::name() const {
    return "app::EnvironmentController";
}
void EnvironmentController::draw() {
    const auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    graphics->bloom_begin(m_bloom);

    draw_ground();

    draw_bench(glm::vec3(12.0f, 0.0f, -1.0f), true);
    draw_bench(glm::vec3(-11.0f, 0.0f, 8.5f), false);
    draw_bench(glm::vec3(-11.0f, 0.0f, -4.5f), false);

    draw_statue();

    draw_skybox();

    // Since the whole scene should be rendered before bloom_blur is called and since the LampController is initialized
    // after EnvironmentController, bloom_blur and bloom_draw will be called after lamps are drawn in LampController...
}

void EnvironmentController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void EnvironmentController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void EnvironmentController::draw_skybox() {
    // Skybox taken from https://drive.google.com/drive/folders/1IRbOo3bMVDwj2NjgSwbSFDWoLHSHGHGe
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resource->skybox("skybox1");
    auto shader = resource->shader("SkyboxShader");
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    graphics->draw_skybox(shader, skybox);
}

void EnvironmentController::draw_ground() {
    // Custom-made model in Blender
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    auto ground = resource->model("ground");
    auto main_shader = resource->shader("MainShader");
    main_shader->use();
    main_shader->set_mat4("projection", graphics->projection_matrix());
    main_shader->set_mat4("view", graphics->camera()->view_matrix());

    auto model = glm::mat4(1.0f);
    model = scale(model, glm::vec3(17.0f, 1.0f, 17.0f));
    main_shader->set_mat4("model", model);

    main_shader->set_vec2("tiling", glm::vec2{17.0f, 17.0f});

    auto gui = engine::graphics::GraphicsController::get<app::GUIController>();
    auto color = gui->color_selection();

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader, color);

    auto lamps = engine::core::Controller::get<LampController>();
    lamps->set_point_lights(main_shader);

    ground->draw(main_shader);
}

void EnvironmentController::draw_bench(glm::vec3 position, bool to_rotate) {
    // Model taken from: https://sketchfab.com/3d-models/park-bench-507aa43cecdb432ebe078b84d8027ac9
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
    auto bench = resource->model("bench");
    auto main_shader = resource->shader("MainShader");
    main_shader->use();
    main_shader->set_mat4("projection", graphics->projection_matrix());
    main_shader->set_mat4("view", graphics->camera()->view_matrix());

    auto model = glm::mat4(1.0f);
    model = translate(model, position);
    if (to_rotate) {
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    main_shader->set_mat4("model", model);

    main_shader->set_vec2("tiling", glm::vec2(1.0f, 1.0f));

    auto gui = engine::graphics::GraphicsController::get<app::GUIController>();
    auto color = gui->color_selection();

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader, color);

    auto lamps = engine::core::Controller::get<LampController>();
    lamps->set_point_lights(main_shader);

    bench->draw(main_shader);
}

void EnvironmentController::draw_statue() {
    // Model taken from: https://sketchfab.com/3d-models/stone-monument-f63da49ec506459fb3da095dcf4f3e45
    auto resource = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();

    auto statue = resource->model("statue");
    auto main_shader = resource->shader("MainShader");
    main_shader->use();
    main_shader->set_mat4("projection", graphics->projection_matrix());
    main_shader->set_mat4("view", graphics->camera()->view_matrix());

    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(1.0f, -0.05f, -12.0f));
    main_shader->set_mat4("model", model);

    main_shader->set_vec2("tiling", glm::vec2(1.0f, 1.0f));

    auto gui = engine::graphics::GraphicsController::get<app::GUIController>();
    auto color = gui->color_selection();

    auto flashlight = engine::core::Controller::get<FlashlightController>();
    flashlight->setup_flashlight(main_shader, color);

    auto lamps = engine::core::Controller::get<LampController>();
    lamps->set_point_lights(main_shader);

    statue->draw(main_shader);
}

std::pair<engine::resources::Shader *, engine::resources::Shader *> EnvironmentController::get_bloom_shaders() const {
    return std::make_pair(m_blur_shader, m_final_shader);
}

}// namespace app