#include "EverEngineCore/rendering/shader/API/OpenGL/OpenGLShader.h"
#include "EverEngineCore/core/Log.h"
#include "EverEngineCore/platform/filesystem/FileSystem.h"  // <- ДОДАЙ ЦЕЙ INCLUDE
#include <vector>

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_name(name)
{
    std::unordered_map<ShaderStageType, std::string> sources;
    sources[ShaderStageType::Vertex] = vertexSrc;
    sources[ShaderStageType::Fragment] = fragmentSrc;
    compile_from_source(sources);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::unordered_map<ShaderStageType, std::string>& sources)
    : m_name(name)
{
    compile_from_source(sources);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::unordered_map<ShaderStageType, std::string>& filePaths, bool fromFiles)
    : m_name(name)
{
    if (fromFiles)
        compile_from_files(filePaths);
    else
        compile_from_source(filePaths);
}

OpenGLShader::~OpenGLShader()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void OpenGLShader::compile_from_files(const std::unordered_map<ShaderStageType, std::string>& filePaths)
{
    std::unordered_map<ShaderStageType, std::string> sources;
    
    for (const auto& [stage, path] : filePaths)
    {
        if (!File::exists(path))
        {
            LOG_ERROR("Shader file not found: {}", path);
            continue;
        }
        
        if (!File::isFile(path))
        {
            LOG_ERROR("Path is not a file: {}", path);
            continue;
        }
        
        std::string source = File::readText(path);
        if (source.empty())
        {
            LOG_ERROR("Failed to read shader file or file is empty: {}", path);
            continue;
        }
        
        sources[stage] = source;
    }
    
    compile_from_source(sources);
}

void OpenGLShader::compile_from_source(const std::unordered_map<ShaderStageType, std::string>& sources)
{
    m_program = glCreateProgram();
    std::vector<GLuint> shaderIDs;
    
    for (const auto& [stage, source] : sources)
    {
        GLenum glType = shader_stage_to_gl(stage);
        GLuint shaderID = compile_shader(glType, source);
        
        if (shaderID != 0)
        {
            glAttachShader(m_program, shaderID);
            shaderIDs.push_back(shaderID);
        }
    }
    
    if (shaderIDs.empty())
    {
        LOG_CRIT("No valid shaders compiled for '{}'", m_name);
        glDeleteProgram(m_program);
        m_program = 0;
        return;
    }
    
    glLinkProgram(m_program);
    check_compile_errors(m_program, "PROGRAM");

    for (GLuint id : shaderIDs)
    {
        glDetachShader(m_program, id);
        glDeleteShader(id);
    }

    reflect_uniforms();
    
    LOG_INFO("Shader '{}' compiled successfully (ID: {})", m_name, m_program);
}

GLuint OpenGLShader::compile_shader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    std::string typeStr;
    switch (type)
    {
        case GL_VERTEX_SHADER:   typeStr = "VERTEX"; break;
        case GL_FRAGMENT_SHADER: typeStr = "FRAGMENT"; break;
        case GL_GEOMETRY_SHADER: typeStr = "GEOMETRY"; break;
        case GL_COMPUTE_SHADER:  typeStr = "COMPUTE"; break;
        case GL_TESS_EVALUATION_SHADER: typeStr = "TESS_EVAL"; break;
        case GL_TESS_CONTROL_SHADER: typeStr = "TESS_CONTROL"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    
    check_compile_errors(shader, typeStr);
    return shader;
}

void OpenGLShader::check_compile_errors(GLuint shader, const std::string& type)
{
    GLint success;
    char infoLog[1024];
    
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Shader compilation error ({}): {}", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Program linking error: {}", infoLog);
        }
    }
}

void OpenGLShader::reflect_uniforms()
{
    GLint count;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count);
    
    for (GLint i = 0; i < count; i++)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        
        glGetActiveUniform(m_program, i, sizeof(name), &length, &size, &type, name);
        
        GLint location = glGetUniformLocation(m_program, name);
        m_uniformLocationCache[name] = location;
        m_uniformTypes[name] = type;
    }
}

void OpenGLShader::bind() const
{
    glUseProgram(m_program);
}

void OpenGLShader::unbind() const
{
    glUseProgram(0);
}

GLint OpenGLShader::get_uniform_location(const std::string& name) const
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    
    GLint location = glGetUniformLocation(m_program, name.c_str());
    
    if (location == -1)
        LOG_WARN("Uniform '{}' not found in shader '{}'", name, m_name);
    
    m_uniformLocationCache[name] = location;
    return location;
}

// Uniform setters
void OpenGLShader::set_bool(const std::string& name, bool value)
{
    glUniform1i(get_uniform_location(name), static_cast<int>(value));
}

void OpenGLShader::set_int(const std::string& name, int value)
{
    glUniform1i(get_uniform_location(name), value);
}

void OpenGLShader::set_int_array(const std::string& name, int* values, uint32_t count)
{
    glUniform1iv(get_uniform_location(name), count, values);
}

void OpenGLShader::set_float(const std::string& name, float value)
{
    glUniform1f(get_uniform_location(name), value);
}

void OpenGLShader::set_float2(const std::string& name, float x, float y)
{
    glUniform2f(get_uniform_location(name), x, y);
}

void OpenGLShader::set_float3(const std::string& name, float x, float y, float z)
{
    glUniform3f(get_uniform_location(name), x, y, z);
}

void OpenGLShader::set_float4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(get_uniform_location(name), x, y, z, w);
}

void OpenGLShader::set_mat2(const std::string& name, const float* value)
{
    glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, value);
}

void OpenGLShader::set_mat3(const std::string& name, const float* value)
{
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, value);
}

void OpenGLShader::set_mat4(const std::string& name, const float* value)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, value);
}

void OpenGLShader::set_vec2(const std::string& name, const glm::vec2& value)
{
    set_float2(name, value.x, value.y);
}

void OpenGLShader::set_vec3(const std::string& name, const glm::vec3& value)
{
    set_float3(name, value.x, value.y, value.z);
}

void OpenGLShader::set_vec4(const std::string& name, const glm::vec4& value)
{
    set_float4(name, value.x, value.y, value.z, value.w);
}

void OpenGLShader::set_mat2(const std::string& name, const glm::mat2& value)
{
    set_mat2(name, &value[0][0]);
}

void OpenGLShader::set_mat3(const std::string& name, const glm::mat3& value)
{
    set_mat3(name, &value[0][0]);
}

void OpenGLShader::set_mat4(const std::string& name, const glm::mat4& value)
{
    set_mat4(name, &value[0][0]);
}

std::vector<std::string> OpenGLShader::get_uniform_names() const
{
    std::vector<std::string> names;
    for (const auto& [name, location] : m_uniformLocationCache)
    {
        names.push_back(name);
    }
    return names;
}

std::string OpenGLShader::get_uniform_type(const std::string& name) const
{
    auto it = m_uniformTypes.find(name);
    if (it == m_uniformTypes.end())
        return "unknown";
    
    switch (it->second)
    {
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_INT: return "int";
        case GL_BOOL: return "bool";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        default: return "unknown";
    }
}

GLenum OpenGLShader::shader_stage_to_gl(ShaderStageType type)
{
    switch (type)
    {
        case ShaderStageType::Vertex:     return GL_VERTEX_SHADER;
        case ShaderStageType::Fragment:   return GL_FRAGMENT_SHADER;
        case ShaderStageType::Geometry:   return GL_GEOMETRY_SHADER;
        case ShaderStageType::Compute:    return GL_COMPUTE_SHADER;
        case ShaderStageType::TessEval:   return GL_TESS_EVALUATION_SHADER;
        case ShaderStageType::TessControl: return GL_TESS_CONTROL_SHADER;
    }
    return 0;
}