#define __TYPE_VERTEX_SHADER__
#version 450 core

struct TextureInfo
{
    int Index;
    int TexCoord;
};

struct NormalTextureInfo
{
    int Index;
    int TexCoord;
    float Scale;
};

struct OcclusionTextureInfo
{
    int Index;
    int TexCoord;
    float Strenght;
};

struct MetallicRoughtness
{
    vec4 BaseColorFactor;
    TextureInfo BaseColorTexture;
    float MetallicFactor;
    float RoughtnessFactor;
    TextureInfo MetallicRoughnessTexture;
};

struct MaterialParams
{
    MetallicRoughtness PbrMetallicRoughness;
    NormalTextureInfo NormalTexture;
    OcclusionTextureInfo OcclusionTexture;
    TextureInfo EmissiveTexture;
    vec3 EmissiveFactor;
    int AlphaMode;
    float AlphaCutoff;
    int DoubleSided;
};

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
};
layout(std140, binding = 1) uniform Model
{
    mat4 u_Transform;
    int u_EntityId;
};

layout(std140, binding = 2) uniform Lights
{
    vec3 lightDir;
};

layout(std140, binding = 3) uniform MaterialParameters
{
    MaterialParams materialParameters;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out flat int v_EntityId;

void main() {
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_EntityId = u_EntityId;
}

#define __TYPE_FRAGMENT_SHADER__
#version 450 core
struct TextureInfo
{
    int Index;
    int TexCoord;
};

struct NormalTextureInfo
{
    int Index;
    int TexCoord;
    float Scale;
};

struct OcclusionTextureInfo
{
    int Index;
    int TexCoord;
    float Strenght;
};

struct MetallicRoughtness
{
    vec4 BaseColorFactor;
    TextureInfo BaseColorTexture;
    float MetallicFactor;
    float RoughtnessFactor;
    TextureInfo MetallicRoughnessTexture;
};

struct MaterialParams
{
    MetallicRoughtness PbrMetallicRoughness;
    NormalTextureInfo NormalTexture;
    OcclusionTextureInfo OcclusionTexture;
    TextureInfo EmissiveTexture;
    vec3 EmissiveFactor;
    int AlphaMode;
    float AlphaCutoff;
    int DoubleSided;
};

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
};
layout(std140, binding = 1) uniform Model
{
    mat4 u_Transform;
    int u_EntityId;
};

layout(std140, binding = 2) uniform Lights
{
    vec3 lightDir;
};

layout(std140, binding = 3) uniform MaterialParameters
{
    MaterialParams materialParameters;
};

layout (location = 0) out vec4 o_Color;
layout (location = 1) out int o_Entity; // -1 = no entity. (for now.)

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in flat int v_EntityId;

layout (binding = 0) uniform sampler2D u_Texture;

void main() {
    //    o_Color = vec4(v_TexCoord, 0.0, 1.0);
    o_Color = texture(u_Texture, v_TexCoord);
//    float entityColor = v_EntityId / 20.0f;
//    o_Color = vec4(entityColor, entityColor, entityColor, 1.0f);
    o_Entity = v_EntityId;
}