#pragma once

#include <cstddef>
#include <memory>

enum class BufferUsage
{
    Static,
    Dynamic,
    Stream
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_data(const void* data, size_t size) = 0;
    virtual void update_data(size_t offset, const void* data, size_t size) = 0;

    virtual size_t get_size() const = 0;

    static std::shared_ptr<VertexBuffer> create(uint32_t size, BufferUsage usage = BufferUsage::Dynamic);
    static std::shared_ptr<VertexBuffer> create(const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);
};