#pragma once

#include "EverEngineCore/core/Log.h"

enum class APIType
{
    None = 0,
    OpenGL,
    Vulkan,
};

class RendererAPI
{
public:
    virtual ~RendererAPI() = default;

    virtual int init(void*(*)(const char*)) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;
};