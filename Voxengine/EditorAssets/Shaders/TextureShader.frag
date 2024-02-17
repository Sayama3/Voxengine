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