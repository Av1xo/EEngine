#include "EverEngineCore/rendering/buffers/BufferLayout.h"

static uint32_t shader_data_type_size(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:     return 4;
    case ShaderDataType::Float2:    return 4 * 2;
    case ShaderDataType::Float3:    return 4 * 3;
    case ShaderDataType::Float4:    return 4 * 4;
    case ShaderDataType::Int:       return 4;
    case ShaderDataType::Int2:      return 4 * 2;
    case ShaderDataType::Int3:      return 4 * 3;
    case ShaderDataType::Int4:      return 4 * 4;
    case ShaderDataType::Mat3:      return 4 * 3 * 3;
    case ShaderDataType::Mat4:      return 4 * 4 * 4;
    case ShaderDataType::Bool:      return 1;
    case ShaderDataType::None:      return 0;
    }
    return 0;
}

BufferElement::BufferElement(ShaderDataType type, const char* name, bool normalized)
    : name(name), type(type), size(shader_data_type_size(type)), offset(0), normalized(normalized)
    {}

size_t BufferElement::get_components_count() const
{
    switch (type)
    {
    case ShaderDataType::Float:     return 1;
    case ShaderDataType::Float2:    return 2;
    case ShaderDataType::Float3:    return 3;
    case ShaderDataType::Float4:    return 4;
    case ShaderDataType::Int:       return 1;
    case ShaderDataType::Int2:      return 2;
    case ShaderDataType::Int3:      return 3;
    case ShaderDataType::Int4:      return 4;
    case ShaderDataType::Mat3:      return 3;
    case ShaderDataType::Mat4:      return 4;
    case ShaderDataType::Bool:      return 1;
    case ShaderDataType::None:      return 0;
    }
    return 0;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_elements(elements)
    {
        calculate_offsets_and_stride();
    }

void BufferLayout::calculate_offsets_and_stride()
{
    uint32_t offset = 0;
    m_stride = 0;

    for (auto& element : m_elements)
    {
        element.offset = offset;
        offset += element.size;
        m_stride += element.size;
    }
}