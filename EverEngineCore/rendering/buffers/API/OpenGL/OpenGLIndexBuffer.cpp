#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLIndexBuffer.h"
#include "EverEngineCore/core/Log.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* indices, size_t count, BufferUsage usage)
    : m_count(count)
{
    glGenBuffers(1, &m_ebo);
    LOG_INFO("INDEX::GEN->{0}", m_ebo);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage_to_gl(usage));
    unbind();
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }
    LOG_INFO("INDEX::DELETE->{0}", m_ebo);
}

void OpenGLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void OpenGLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum OpenGLIndexBuffer::usage_to_gl(BufferUsage usage)
{
    switch (usage)
    {
    case BufferUsage::Static:   return GL_STATIC_DRAW;
    case BufferUsage::Dynamic:  return GL_DYNAMIC_DRAW;
    case BufferUsage::Stream:   return GL_STREAM_DRAW;
    }
    return GL_STATIC_DRAW;
}