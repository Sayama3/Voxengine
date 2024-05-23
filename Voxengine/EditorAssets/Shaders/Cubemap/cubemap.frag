#version 450 core

layout(location = 0) in vec3 v_TexCoords;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform samplerCube u_Skybox;

void main()
{
    o_Color = texture(u_Skybox, v_TexCoords);
}