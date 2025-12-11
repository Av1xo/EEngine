#pragma once

#include <cstdint>
#include "EverEngineCore/rendering/buffers/VertexBuffer.h"
#include "glad/glad.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(const void* data, size_t size, BufferUsage usage);
    OpenGLVertexBuffer(size_t size, BufferUsage usage);
    virtual ~OpenGLVertexBuffer();

    void bind() const override;
    void unbind() const override;

    void set_data(const void* data, size_t size) override;
    void update_data(size_t offset, const void* data, size_t size) override;

    size_t get_size() const override { return m_size; }
    GLuint get_id() const { return m_vbo; }
private:
    GLuint m_vbo = 0;
    size_t m_size = 0;
    BufferUsage m_usage;

    GLenum usage_to_gl(BufferUsage usage);
};
