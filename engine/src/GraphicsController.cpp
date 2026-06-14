
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <engine/graphics/GraphicsController.hpp>

#include <engine/graphics/Bloom.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Skybox.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine::graphics {

void GraphicsController::initialize() {
    const int opengl_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    RG_GUARANTEE(opengl_initialized, "OpenGL failed to init!");

    auto platform = engine::core::Controller::get<platform::PlatformController>();
    auto handle = platform->window()->handle_();
    m_perspective_params.FOV = glm::radians(m_camera.Zoom);
    m_perspective_params.Width = static_cast<float>(platform->window()->width());
    m_perspective_params.Height = static_cast<float>(platform->window()->height());
    m_perspective_params.Near = 0.1f;
    m_perspective_params.Far = 100.f;
    m_ortho_params.Bottom = 0.0f;
    m_ortho_params.Top = static_cast<float>(platform->window()->height());
    m_ortho_params.Left = 0.0f;
    m_ortho_params.Right = static_cast<float>(platform->window()->width());
    m_ortho_params.Near = 0.1f;
    m_ortho_params.Far = 100.0f;

    platform->register_platform_event_observer(std::make_unique<GraphicsPlatformEventObserver>(this));
    CHECKED_GL_CALL(glViewport, 0, 0, platform->window()->width(), platform->window()->height());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    RG_GUARANTEE(ImGui_ImplGlfw_InitForOpenGL(handle, true), "ImGUI failed to initialize for OpenGL");
    RG_GUARANTEE(ImGui_ImplOpenGL3_Init("#version 330 core"), "ImGUI failed to initialize for OpenGL");
}

void GraphicsController::terminate() {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    m_bloom->destroy();
}

void GraphicsPlatformEventObserver::on_window_resize(int width, int height) {
    m_graphics->perspective_params().Width = static_cast<float>(width);
    m_graphics->perspective_params().Height = static_cast<float>(height);
    m_graphics->orthographic_params().Right = static_cast<float>(width);
    m_graphics->orthographic_params().Top = static_cast<float>(height);
    CHECKED_GL_CALL(glViewport, 0, 0, width, height);
}

std::string_view GraphicsController::name() const {
    return "GraphicsController";
}

void GraphicsController::begin_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GraphicsController::end_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicsController::draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox) {
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.view_matrix()));
    shader->use();
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection_matrix<>());
    CHECKED_GL_CALL(glDepthFunc, GL_LEQUAL);
    CHECKED_GL_CALL(glBindVertexArray, skybox->vao());
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, skybox->texture());
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 36);
    CHECKED_GL_CALL(glBindVertexArray, 0);
    CHECKED_GL_CALL(glDepthFunc, GL_LESS);// set depth function back to default
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}

Bloom *GraphicsController::bloom_init(int width, int height) {
    m_bloom = std::make_unique<Bloom>(width, height);
    return m_bloom.get();
}

void GraphicsController::bloom_begin(const graphics::Bloom *bloom) {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, bloom->m_hdr_fbo);
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsController::bloom_blur(const resources::Shader *blur_shader, const graphics::Bloom *bloom) {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

    bool horizontal = true;
    bool first_iteration = true;
    int iterations = 10;

    blur_shader->use();

    for (int i = 0; i < iterations; i++) {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, bloom->m_pingpong_fbos[horizontal]);
        CHECKED_GL_CALL(glViewport, 0, 0, bloom->m_width, bloom->m_height);
        blur_shader->set_bool("horizontal", horizontal);
        blur_shader->set_int("image", 0);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, first_iteration ? bloom->m_color_buffers[1] : bloom->m_pingpong_color_buffers[!horizontal]);

        // render quad
        CHECKED_GL_CALL(glBindVertexArray, bloom->m_quad_vao);
        CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
        CHECKED_GL_CALL(glBindVertexArray, 0);

        horizontal = !horizontal;
        if (first_iteration) {
            first_iteration = false;
        }
    }
    bloom->m_horizontal = horizontal;
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

void GraphicsController::bloom_draw(const resources::Shader *final_shader, const graphics::Bloom *bloom) {
    CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    final_shader->use();
    final_shader->set_int("scene", 0);
    final_shader->set_int("bloomBlur", 1);
    final_shader->set_float("exposure", 0.15f);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, bloom->m_color_buffers[0]);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE1);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, bloom->m_pingpong_color_buffers[!bloom->m_horizontal]);

    // render quad
    CHECKED_GL_CALL(glBindVertexArray, bloom->m_quad_vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
    CHECKED_GL_CALL(glBindVertexArray, 0);
}

}// namespace engine::graphics
