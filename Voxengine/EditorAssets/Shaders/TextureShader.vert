#version 450 core

struct TextureInfo
{
    int Index;
    int TexCoord;
    int Padding1;
    int Padding2;
};

struct NormalTextureInfo
{
    int Index;
    int TexCoord;
    float Scale;
    int Padding1;
};

struct OcclusionTextureInfo
{
    int Index;
    int TexCoord;
    float Strenght;
    int Padding1;
};

struct MetallicRoughtness
{
    vec4 BaseColorFactor;
    TextureInfo BaseColorTexture;
    TextureInfo MetallicRoughnessTexture;
    float MetallicFactor;
    float RoughtnessFactor;
    int Padding1;
    int Padding2;
};

struct MaterialParams
{
    MetallicRoughtness PbrMetallicRoughness;
    NormalTextureInfo NormalTexture;
    OcclusionTextureInfo OcclusionTexture;
    TextureInfo EmissiveTexture;
    vec4 EmissiveFactor;
    int AlphaMode; // Opaque = 0, Mask = 1, Blend = 2,
    float AlphaCutoff;
    int DoubleSided;
    int GammaCorrection;
};

#define MAX_LIGHT_COUNT 20
struct Light
{
    vec4 Color;
    vec4 Position;
    vec4 Direction;
    float Range;
    float Intensity;
    int Type; //0 = Directional ; 1 = Point ; 2 = Spot
};

struct LightData
{
    Light lights[MAX_LIGHT_COUNT];
//    Light lights;
    int lightCount;
};

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    vec4 u_CameraPosition;
    vec4 u_CameraDirection;
};

layout(std140, binding = 1) uniform Model
{
    mat4 u_ModelMatrix;
    mat4 u_NormalMatrix;
    int u_EntityId;
};

layout(std140, binding = 2) uniform Lights
{
    LightData lights;
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
    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_EntityId = u_EntityId;
}
