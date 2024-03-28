#version 450 core

#define ALPHA_MODE_OPAQUE 0
#define ALPHA_MODE_MASK 1
#define ALPHA_MODE_BLEND 2

#define MAX_LIGHT_COUNT 20
#define EPSILON 0.1

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
    vec4 EmissiveFactor;
    int AlphaMode;
    float AlphaCutoff;
    int DoubleSided;
};

struct Light
{
    vec4 Color;
    vec4 Position;
    vec4 Direction;
    float Range;
    float Intensity;
    int Type; //0 = Directional ; 2 = Spot ; 1 = Point
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

layout (binding = 0) uniform sampler2D u_Textures[32];

bool approx(float a, float b)
{
    return abs(b-a) < EPSILON;
}


void main()
{
    o_Entity = u_EntityId;
    o_Color = materialParameters.PbrMetallicRoughness.BaseColorFactor;

//    if(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index > -1)
//    {
//        o_Color *= SampleTexture(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index);
//        if(materialParameters.PbrMetallicRoughness.BaseColorTexture.TexCoord > 0)
//        {
//            o_Color = vec4(0.8, 0.2, 0.3, 1.0);
//        }
//    }

    if(float(materialParameters.AlphaMode) == float(ALPHA_MODE_MASK) && o_Color.a <= materialParameters.AlphaCutoff) discard;
}