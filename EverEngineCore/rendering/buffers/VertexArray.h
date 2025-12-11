#pragma once

#include "EverEngineCore/rendering/buffers/VertexBuffer.h"
#include "EverEngineCore/rendering/buffers/IndexBuffer.h"
#include "EverEngineCore/rendering/buffers/BufferLayout.h"

enum class DrawMode
{
    Triangles,
    Lines,
    Points,
    Patches,
    TriangleStrip,
    LineStrip,
};

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void draw(DrawMode mode = DrawMode::Triangles) const = 0;

    virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vbo, const BufferLayout& layout) = 0;
    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& ebo) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer>&  get_index_buffer() const = 0;

    static std::shared_ptr<VertexArray> create();
};