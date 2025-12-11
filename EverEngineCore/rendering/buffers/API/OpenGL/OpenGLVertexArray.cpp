#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLIndexBuffer.h"
#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLVertexBuffer.h"
#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLVertexArray.h"
#include "EverEngineCore/core/Log.h"

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_vao);
    LOG_INFO("VAO::CREATED->{0}", m_vao);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    LOG_INFO("VAO::DELETED->{0}", m_vao);
}

void OpenGLVertexArray::bind() const
{
    glBindVertexArray(m_vao);
}

void OpenGLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::draw(DrawMode mode) const
{
    bind();

    GLenum glMode = GL_TRIANGLES;
    switch(mode)
    {
    case DrawMode::Triangles:     glMode = GL_TRIANGLES; break;
    case DrawMode::Lines:         glMode = GL_LINES; break;
    case DrawMode::Points:        glMode = GL_POINTS; break;
    case DrawMode::TriangleStrip: glMode = GL_TRIANGLE_STRIP; break;
    case DrawMode::LineStrip:     glMode = GL_LINE_STRIP; break;
    case DrawMode::Patches:       glMode = GL_PATCHES; break;
    }

    if(m_indexBuffer)
    {
        glDrawElements(glMode, m_indexBuffer->get_count(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {

    }
    unbind();
}

void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vbo, const BufferLayout& layout)
{
    if (layout.get_elements().empty())
    {
        LOG_ERROR("ERROR::VBO::NO_LAYOUT");
        return;
    }

    bind();
    vbo->bind();

    for (const auto& element : layout.get_elements())
    {
        glEnableVertexAttribArray(m_vertexBufferIndex);
        glVertexAttribPointer(
            m_vertexBufferIndex,
            element.get_components_count(),
            shader_type_to_gl(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            layout.get_stride(),
            (const void*)(intptr_t)element.offset
        );
        m_vertexBufferIndex++;
    }

    m_vertexBuffers.push_back(vbo);
    LOG_INFO("VBO::ADDED");
    m_vertexCount = vbo->get_size() / layout.get_stride();
    unbind();
}

void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& ebo)
{
    bind();
    ebo->bind();
    m_indexBuffer = ebo;
    unbind();
    LOG_INFO("EBO::SET::SUCCESSFUL");
}

GLenum OpenGLVertexArray::shader_type_to_gl(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;
        
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
        
    case ShaderDataType::Bool:
        return GL_BOOL;
        
    case ShaderDataType::None:
        return 0;
    }
    return 0;
}



