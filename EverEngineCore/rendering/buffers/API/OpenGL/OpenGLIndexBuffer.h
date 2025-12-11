#pragma once

#include "EverEngineCore/rendering/buffers/IndexBuffer.h"
#include "glad/glad.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(const unsigned int* indices, size_t count, BufferUsage usage);
    virtual ~OpenGLIndexBuffer();

    void bind() const override;
    void unbind() const override;

    size_t get_count() const override { return m_count; }
    GLuint get_id() const { return m_ebo; }

private:
    GLuint m_ebo = 0;
    size_t m_count = 0;

    GLenum usage_to_gl(BufferUsage usage);
};