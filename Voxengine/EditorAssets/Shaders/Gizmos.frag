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

layout(std140, binding = 3) uniform MaterialParameters
{
    MaterialParams materialParameters;
};

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_Color;

layout (location = 0) out vec4 o_Color;

void main() {
    o_Color = materialParameters.PbrMetallicRoughness.BaseColorFactor;
//    o_Color = v_Color;
}