#version 450 core

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    vec4 u_CameraPosition;
    vec4 u_CameraDirection;
};

layout (location = 0) in vec3 v_Position;

layout(location = 0) out vec3 v_TexCoords;

void main()
{
    v_TexCoords = v_Position;
    gl_Position = u_ViewProjectionMatrix * vec4(v_Position, 1.0);
}