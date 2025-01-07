#version 450 core

#define ALPHA_MODE_OPAQUE 0
#define ALPHA_MODE_MASK 1
#define ALPHA_MODE_BLEND 2

#define MAX_LIGHT_COUNT 20
#define EPSILON 0.1

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

void main() {
    o_Position = vec4(v_Position, 1);
    o_Normal = vec4(v_Normal, 1);
    o_TexCoord = vec4(v_TexCoord, 1 ,1);
    o_Color = v_Color;
    o_EntityId = u_EntityId;
}