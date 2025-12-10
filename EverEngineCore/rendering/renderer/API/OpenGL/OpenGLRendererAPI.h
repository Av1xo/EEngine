#pragma once
#include "EverEngineCore/rendering/renderer/API/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
    int init(void*(*)(const char*)) override;
    void setClearColor(float r, float g, float b, float a) override;
    void clear() override;
};