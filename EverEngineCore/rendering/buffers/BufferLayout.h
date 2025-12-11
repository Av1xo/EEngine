#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

enum class ShaderDataType
{
    None = 0,
    Float, Float2, Float3, Float4,
    Int, Int2, Int3, Int4,
    Mat3, Mat4,
    Bool
};

struct BufferElement
{
    const char* name;
    ShaderDataType type;
    uint32_t size;
    uint32_t offset;
    bool normalized;

    BufferElement() = default;
    BufferElement(ShaderDataType type, const char* name, bool normalized = false);

    size_t get_components_count() const;
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements);

    const std::vector<BufferElement>& get_elements() const { return m_elements; }
    uint32_t get_stride() const { return m_stride; }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

private:
    void calculate_offsets_and_stride();
    
    std::vector<BufferElement> m_elements;
    uint32_t  m_stride = 0;
};