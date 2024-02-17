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
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec4 v_Color;
layout(location = 2) out flat int v_EntityId;

void main()
{
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
    v_Position = (u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0)).xyz;
    v_Color = a_Color;
    v_EntityId = u_EntityId;
}