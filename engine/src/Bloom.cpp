//
// Created by filip on 14. 5. 2026..
//

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Bloom.hpp>

namespace engine::resources {

void Bloom::destroy() {
    CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_hdr_fbo);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_color_buffers);
    CHECKED_GL_CALL(glDeleteFramebuffers, 2, m_pingpong_fbos);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_pingpong_color_buffers);
    CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad_vao);
    CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad_vbo);
}

}// namespace engine::resources