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
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec4 v_Color;
layout (location = 3) out flat int v_EntityId;

void main() {
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);

    v_Position = (u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0)).xyz;
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
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

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec4 v_Color;
layout (location = 3) in flat int v_EntityId;

layout (binding = 0) uniform sampler2D u_Textures[32];

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

void main()
{
    o_Color = materialParameters.PbrMetallicRoughness.BaseColorFactor;

    if(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index > -1)
    {
        o_Color *= SampleTexture(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index);
        if(materialParameters.PbrMetallicRoughness.BaseColorTexture.TexCoord > 0)
        {
            o_Color = vec4(0.8,0.2,0.3,1.0);
        }
    }

    o_Entity = v_EntityId;
}