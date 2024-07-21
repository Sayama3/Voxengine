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