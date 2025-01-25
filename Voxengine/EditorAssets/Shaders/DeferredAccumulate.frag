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

layout(std140, binding = 3) uniform MaterialParameters
{
    MaterialParams materialParameters;
};
layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_Color;
layout (location = 4) in flat int v_EntityId;

layout (location = 0) out vec4 o_Position;
layout (location = 1) out vec4 o_Normal;
layout (location = 2) out vec4 o_TexCoord;
layout (location = 3) out vec4 o_Color;
layout (location = 4) out int o_EntityId; // -1 = no entity. (for now.)
layout (binding = 0) uniform sampler2D u_Textures[32];

bool approx(float a, float b)
{
    return abs(b-a) < EPSILON;
}

vec4 SampleTexture(int texIndex)
{
    vec4 texColor = vec4(1,0,1,1);

    switch(int(texIndex))
    {
        case  0: texColor = texture(u_Textures[ 0], v_TexCoord /** Input.TilingFactor*/); break;
        case  1: texColor = texture(u_Textures[ 1], v_TexCoord /** Input.TilingFactor*/); break;
        case  2: texColor = texture(u_Textures[ 2], v_TexCoord /** Input.TilingFactor*/); break;
        case  3: texColor = texture(u_Textures[ 3], v_TexCoord /** Input.TilingFactor*/); break;
        case  4: texColor = texture(u_Textures[ 4], v_TexCoord /** Input.TilingFactor*/); break;
        case  5: texColor = texture(u_Textures[ 5], v_TexCoord /** Input.TilingFactor*/); break;
        case  6: texColor = texture(u_Textures[ 6], v_TexCoord /** Input.TilingFactor*/); break;
        case  7: texColor = texture(u_Textures[ 7], v_TexCoord /** Input.TilingFactor*/); break;
        case  8: texColor = texture(u_Textures[ 8], v_TexCoord /** Input.TilingFactor*/); break;
        case  9: texColor = texture(u_Textures[ 9], v_TexCoord /** Input.TilingFactor*/); break;
        case 10: texColor = texture(u_Textures[10], v_TexCoord /** Input.TilingFactor*/); break;
        case 11: texColor = texture(u_Textures[11], v_TexCoord /** Input.TilingFactor*/); break;
        case 12: texColor = texture(u_Textures[12], v_TexCoord /** Input.TilingFactor*/); break;
        case 13: texColor = texture(u_Textures[13], v_TexCoord /** Input.TilingFactor*/); break;
        case 14: texColor = texture(u_Textures[14], v_TexCoord /** Input.TilingFactor*/); break;
        case 15: texColor = texture(u_Textures[15], v_TexCoord /** Input.TilingFactor*/); break;
        case 16: texColor = texture(u_Textures[16], v_TexCoord /** Input.TilingFactor*/); break;
        case 17: texColor = texture(u_Textures[17], v_TexCoord /** Input.TilingFactor*/); break;
        case 18: texColor = texture(u_Textures[18], v_TexCoord /** Input.TilingFactor*/); break;
        case 19: texColor = texture(u_Textures[19], v_TexCoord /** Input.TilingFactor*/); break;
        case 20: texColor = texture(u_Textures[20], v_TexCoord /** Input.TilingFactor*/); break;
        case 21: texColor = texture(u_Textures[21], v_TexCoord /** Input.TilingFactor*/); break;
        case 22: texColor = texture(u_Textures[22], v_TexCoord /** Input.TilingFactor*/); break;
        case 23: texColor = texture(u_Textures[23], v_TexCoord /** Input.TilingFactor*/); break;
        case 24: texColor = texture(u_Textures[24], v_TexCoord /** Input.TilingFactor*/); break;
        case 25: texColor = texture(u_Textures[25], v_TexCoord /** Input.TilingFactor*/); break;
        case 26: texColor = texture(u_Textures[26], v_TexCoord /** Input.TilingFactor*/); break;
        case 27: texColor = texture(u_Textures[27], v_TexCoord /** Input.TilingFactor*/); break;
        case 28: texColor = texture(u_Textures[28], v_TexCoord /** Input.TilingFactor*/); break;
        case 29: texColor = texture(u_Textures[29], v_TexCoord /** Input.TilingFactor*/); break;
        case 30: texColor = texture(u_Textures[30], v_TexCoord /** Input.TilingFactor*/); break;
        case 31: texColor = texture(u_Textures[31], v_TexCoord /** Input.TilingFactor*/); break;
    }
    return texColor;
}
void main() {
    o_Position = vec4(v_Position, 1);
    o_Normal = vec4(v_Normal, 1);
    o_TexCoord = vec4(v_TexCoord, 1 ,1);
    o_Color = materialParameters.PbrMetallicRoughness.BaseColorFactor;
    o_EntityId = u_EntityId;
/*
    if(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index > -1)
    {
        o_Color *= SampleTexture(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index);
        if(materialParameters.PbrMetallicRoughness.BaseColorTexture.TexCoord > 0)
        {
            o_Color = vec4(0.8, 0.2, 0.3, 1.0);
        }
    }
*/
}