#pragma once

#include "EverEngineCore/rendering/shader/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    OpenGLShader(const std::string& name, const std::unordered_map<ShaderStageType, std::string>& sources);
    OpenGLShader(const std::string& name, const std::unordered_map<ShaderStageType, std::string>& filePaths, bool fromFiles);
    
    virtual ~OpenGLShader();

    void bind() const override;
    void unbind() const override;
    
    bool is_valid() const override { return m_program != 0; }
    uint32_t get_id() const override { return m_program; }
    const std::string& get_name() const override { return m_name; }

    void set_bool(const std::string& name, bool value) override;
    void set_int(const std::string& name, int value) override;
    void set_int_array(const std::string& name, int* values, uint32_t count) override;
    
    void set_float(const std::string& name, float value) override;
    void set_float2(const std::string& name, float x, float y) override;
    void set_float3(const std::string& name, float x, float y, float z) override;
    void set_float4(const std::string& name, float x, float y, float z, float w) override;
    
    void set_mat2(const std::string& name, const float* value) override;
    void set_mat3(const std::string& name, const float* value) override;
    void set_mat4(const std::string& name, const float* value) override;

    void set_vec2(const std::string& name, const glm::vec2& value);
    void set_vec3(const std::string& name, const glm::vec3& value);
    void set_vec4(const std::string& name, const glm::vec4& value);
    void set_mat2(const std::string& name, const glm::mat2& value);
    void set_mat3(const std::string& name, const glm::mat3& value);
    void set_mat4(const std::string& name, const glm::mat4& value);

    std::vector<std::string> get_uniform_names() const override;
    std::string get_uniform_type(const std::string& name) const override;

private:
    GLuint m_program = 0;
    std::string m_name;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;
    mutable std::unordered_map<std::string, GLenum> m_uniformTypes;

    void compile_from_source(const std::unordered_map<ShaderStageType, std::string>& sources);
    void compile_from_files(const std::unordered_map<ShaderStageType, std::string>& filePaths);
    
    GLuint compile_shader(GLenum type, const std::string& source);
    void check_compile_errors(GLuint shader, const std::string& type);
    void reflect_uniforms();
    
    GLint get_uniform_location(const std::string& name) const;
    GLenum shader_stage_to_gl(ShaderStageType type);
};