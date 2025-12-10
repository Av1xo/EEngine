#pragma once
#include <memory>
#include "EverEngineCore/rendering/renderer/API/RendererAPI.h"

class Renderer
{
public:
    static int init(void*(*loader)(const char*), APIType api = APIType::OpenGL);
    static void setClearColor(float r, float g, float b, float a);
    static void clear();
private:
    static std::unique_ptr<RendererAPI> m_api;    
};