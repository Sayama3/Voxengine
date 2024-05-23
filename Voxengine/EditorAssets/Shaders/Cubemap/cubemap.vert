#version 450 core

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

layout (location = 0) in vec3 a_Position;
layout(location = 0) out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;
    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}