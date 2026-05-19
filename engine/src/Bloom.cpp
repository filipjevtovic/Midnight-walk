//
// Created by filip on 14. 5. 2026..
//

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Bloom.hpp>

namespace engine::resources {

Bloom::Bloom(int32_t width, int32_t height)
    : m_width(width)
    , m_height(height) {
    glGenFramebuffers(1, &this->m_hdr_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_hdr_fbo);

    glGenTextures(2, this->m_color_buffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, this->m_color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->m_color_buffers[i], 0);
    }

    glGenRenderbuffers(1, &this->m_depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_depth_rbo);

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::info("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, this->m_pingpong_fbos);
    glGenTextures(2, this->m_pingpong_color_buffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_pingpong_fbos[i]);
        glBindTexture(GL_TEXTURE_2D, this->m_pingpong_color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_pingpong_color_buffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            spdlog::info("Pingpong framebuffer not complete!");
        }
    }

    // clang-format off
        float quad_vertices[] = {
                //position         //texture coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // clang-format: on
        glGenVertexArrays(1, &this->m_quad_vao);
        glGenBuffers(1, &this->m_quad_vbo);

        glBindVertexArray(this->m_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_quad_vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

void Bloom::destroy() {
    CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_hdr_fbo);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_color_buffers);
    CHECKED_GL_CALL(glDeleteFramebuffers, 2, m_pingpong_fbos);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_pingpong_color_buffers);
    CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad_vao);
    CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad_vbo);
}

}// namespace engine::resources