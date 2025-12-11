#include "EverEngineCore/rendering/buffers/IndexBuffer.h"
#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLIndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(const unsigned int* indices, size_t count, BufferUsage usage)
{
    /** TODO: add dynamic API choose*/ 
    return std::make_shared<OpenGLIndexBuffer>(indices, count, usage);
}

