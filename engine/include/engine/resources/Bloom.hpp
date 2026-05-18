//
// Created by filip on 14. 5. 2026..
//

#ifndef MATF_RG_PROJECT_BLOOM_HPP
#define MATF_RG_PROJECT_BLOOM_HPP

#include <engine/graphics/GraphicsController.hpp>

namespace engine::resources {
// TODO: Add doxygen annotations later
class Bloom {
    friend class ResourcesController;
    friend class engine::graphics::GraphicsController;

public:
    Bloom() = delete;
    Bloom(int32_t width, int32_t height)
        : m_width(width)
        , m_height(height) {}

    void destroy();

private:
    uint32_t m_hdr_fbo{0};
    uint32_t m_color_buffers[2]{};
    uint32_t m_depth_rbo{0};
    uint32_t m_pingpong_fbos[2]{};
    uint32_t m_pingpong_color_buffers[2]{};

    uint32_t m_quad_vao{0};
    uint32_t m_quad_vbo{0};

    uint32_t m_width{0};
    uint32_t m_height{0};

    mutable bool m_horizontal{false};
};
}// namespace engine::resources

#endif//MATF_RG_PROJECT_BLOOM_HPP