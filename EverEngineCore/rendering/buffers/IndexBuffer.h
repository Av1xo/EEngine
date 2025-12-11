#pragma once

#include <cstddef>

#include "EverEngineCore/rendering/buffers/VertexBuffer.h"

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual size_t get_count() const = 0;

    static std::shared_ptr<IndexBuffer> create(const unsigned int* indices, size_t count, BufferUsage usage = BufferUsage::Static);
};