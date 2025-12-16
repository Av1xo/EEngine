#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

enum class ShaderStageType
{
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessEval,
    TessControl
};

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual bool is_valid() const = 0;
    virtual uint32_t get_id() const = 0;
    virtual const std::string& get_name() const = 0;

    virtual void set_bool(const std::string& name, bool value) = 0;
    virtual void set_int(const std::string& name, int value) = 0;
    virtual void set_int_array(const std::string& name, int* values, uint32_t count) = 0;
    
    virtual void set_float(const std::string& name, float value) = 0;
    virtual void set_float2(const std::string& name, float x, float y) = 0;
    virtual void set_float3(const std::string& name, float x, float y, float z) = 0;
    virtual void set_float4(const std::string& name, float x, float y, float z, float w) = 0;
    
    virtual void set_mat2(const std::string& name, const float* value) = 0;
    virtual void set_mat3(const std::string& name, const float* value) = 0;
    virtual void set_mat4(const std::string& name, const float* value) = 0;

    virtual std::vector<std::string> get_uniform_names() const = 0;
    virtual std::string get_uniform_type(const std::string& name) const = 0;
    

    static std::shared_ptr<Shader> create_from_files(
        const std::string& name,
        const std::unordered_map<ShaderStageType, std::string>& filePaths
    );

    static std::shared_ptr<Shader> create_from_source(
        const std::string& name,
        const std::unordered_map<ShaderStageType, std::string>& sources
    );
};