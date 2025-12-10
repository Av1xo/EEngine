#include "EverEngineCore/rendering/renderer/Renderer.h"
#include "EverEngineCore/rendering/renderer/API/OpenGL/OpenGLRendererAPI.h"

std::unique_ptr<RendererAPI> Renderer::m_api = nullptr;

int Renderer::init(void*(*loader)(const char*), APIType api)
{
    switch (api)
    {
    case APIType::OpenGL:
        m_api = std::make_unique<OpenGLRendererAPI>();
        break;
    
    default:
        LOG_ERROR("ERROR::UNSUPPORTED::RENDERER_API");
        return -1;
    }
    return m_api->init(loader);
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
    if (!m_api)
    {
        LOG_WARN("WARN::API::NOT_INITIALIZED");
        return;
    }
    m_api->setClearColor(r, g, b, a);
}

void Renderer::clear()
{
    if (!m_api)
    {
        LOG_WARN("WARN::API::NOT_INITIALIZED");
        return;
    }

    m_api->clear();
}

