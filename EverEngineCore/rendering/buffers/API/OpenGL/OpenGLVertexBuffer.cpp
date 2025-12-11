#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLVertexBuffer.h"
#include "EverEngineCore/core/Log.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, size_t size, BufferUsage usage)
    : m_size(size),  m_usage(usage)
{
    glGenBuffers(1, &m_vbo);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_gl(usage));
    unbind();
}

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, BufferUsage usage)
    : m_size(size), m_usage(usage)
{
    glGenBuffers(1, &m_vbo);
    LOG_INFO("VERTEX::GEN->{0}", m_vbo);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage_to_gl(usage));
    unbind();
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    LOG_INFO("VERTEX::DELETE->{0}", m_vbo);
}

void OpenGLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void OpenGLVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::set_data(const void* data, size_t size)
{
    m_size = size;
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_gl(m_usage));
    unbind();
}

void OpenGLVertexBuffer::update_data(size_t offset, const void* data, size_t size)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    unbind();
}

GLenum OpenGLVertexBuffer::usage_to_gl(BufferUsage usage)
{
    switch (usage)
    {
    case BufferUsage::Static:   return GL_STATIC_DRAW;
    case BufferUsage::Dynamic:  return GL_DYNAMIC_DRAW;
    case BufferUsage::Stream:   return GL_STREAM_DRAW;
    }
    return GL_STATIC_DRAW;
}