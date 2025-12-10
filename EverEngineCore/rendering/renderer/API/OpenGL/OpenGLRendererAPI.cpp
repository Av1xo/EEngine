#include "EverEngineCore/rendering/renderer/API/OpenGL/OpenGLRendererAPI.h"
#include <glad/glad.h>

int OpenGLRendererAPI::init(void*(*loadProc)(const char*))
{
    if(!gladLoadGLLoader((GLADloadproc)loadProc))
    {
        LOG_CRIT("FAIL::INIT::GLAD");
        return -1;
    }
    return 0;
};

void OpenGLRendererAPI::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGLRendererAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}