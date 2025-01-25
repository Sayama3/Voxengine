#version 450

struct Light {
    vec4 Color;
    vec4 Position;
    uint Seed;
};

struct GPUData {
    Light lightPositions[10];
    float timeStep;
    uint lightCount;
};

layout (std430, binding=0) buffer shader_data
{
    GPUData data;
};

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    vec4 u_CameraPosition;
    vec4 u_CameraDirection;
};

layout (location = 0) out vec3 position;
layout (location = 1) out vec4 color;

void main() {
    Light light = data.lightPositions[gl_VertexIndex];
    position = light.Position.xyz;
    gl_Position = vec4(light.Position.xyz, 1);
    color = light.Color;
}