#include "EverEngineCore/rendering/shader/Shader.h"
#include "EverEngineCore/rendering/shader/API/OpenGL/OpenGLShader.h"
#include "EverEngineCore/core/Log.h"

std::shared_ptr<Shader> Shader::create_from_files(
    const std::string& name,
    const std::unordered_map<ShaderStageType, std::string>& filePaths)
{
    return std::make_shared<OpenGLShader>(name, filePaths, true);
}

std::shared_ptr<Shader> Shader::create_from_source(
    const std::string& name,
    const std::unordered_map<ShaderStageType, std::string>& sources)
{
    return std::make_shared<OpenGLShader>(name, sources);
}