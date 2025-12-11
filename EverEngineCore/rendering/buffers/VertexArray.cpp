#include "EverEngineCore/rendering/buffers/VertexArray.h"
#include "EverEngineCore/rendering/buffers/API/OpenGL/OpenGLVertexArray.h"

std::shared_ptr<VertexArray> VertexArray::create()
{
    /** TODO: add dynamic API choose*/ 
    return std::make_shared<OpenGLVertexArray>();
}