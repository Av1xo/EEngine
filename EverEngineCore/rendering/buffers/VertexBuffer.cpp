#include "EverEngineCore/rendering/buffers/VertexBuffer.h"
#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLVertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size, BufferUsage usage)
{
    /** TODO: add dynamic API choose*/ 
    return std::make_shared<OpenGLVertexBuffer>(size, usage);
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* data, uint32_t size, BufferUsage usage)
{
    /** TODO: add dynamic API choose*/ 
    return std::make_shared<OpenGLVertexBuffer>(data, size, usage);
}