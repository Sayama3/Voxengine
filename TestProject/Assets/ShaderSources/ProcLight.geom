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

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoord;
layout (location = 3) out vec4 v_Color;

layout (location = 0) in vec3 position[];
layout (location = 1) in vec4 color[];

vec4 A = vec4(-0.5,-0.5,-0.5, 0);
vec4 B = vec4(+0.5,-0.5,-0.5, 0);
vec4 C = vec4(+0.0,+0.5,+0.0, 0);
vec4 D = vec4(+0.0,-0.5,+0.5, 0);

void BuildVertex(vec4 position, vec4 offset, vec2 texcoord)
{
    v_Position = (position + offset).xyz;
    v_Normal = normalize(offset.xyz);
    v_TexCoord = texcoord;
    gl_Position = u_ViewProjectionMatrix * vec4(v_Position, 1);
}

void buildPiramid(vec4 position)
{
    BuildVertex(position, B, vec2(0.0,0.0));
    EmitVertex();
    BuildVertex(position, A, vec2(0.2,1.0));
    EmitVertex();
    BuildVertex(position, C, vec2(0.4,0.0));
    EmitVertex();
    BuildVertex(position, D, vec2(0.6,1.0));
    EmitVertex();
    BuildVertex(position, B, vec2(0.8,0.0));
    EmitVertex();
    BuildVertex(position, A, vec2(1.0,1.0));
    EmitVertex();
}

void main() {
    v_Color = color[0];
    buildPiramid(vec4(position[0],1));
}