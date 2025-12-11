#pragma once

#include "EverEngineCore/rendering/buffers/VertexArray.h"
#include <glad/glad.h>

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void bind() const override;
    void unbind() const override;
    void draw(DrawMode mode = DrawMode::Triangles) const override;

    void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vbo, const BufferLayout& layout) override;
    void set_index_buffer(const std::shared_ptr<IndexBuffer>& ebo) override;

    const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const override { return m_vertexBuffers; }
    const std::shared_ptr<IndexBuffer>& get_index_buffer() const override { return m_indexBuffer; }

    GLuint get_id() const { return m_vao; }
private:
    GLuint m_vao = 0;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    GLuint m_vertexBufferIndex = 0;
    size_t m_vertexCount = 0;
    GLenum shader_type_to_gl(ShaderDataType type);
};