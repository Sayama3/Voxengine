#version 450 core

layout(location = 0) in vec3 v_TexCoords;

layout(location = 0) out vec4 o_Color;
layout (location = 1) out int o_Entity; // -1 = no entity. (for now.)

layout(binding = 0) uniform samplerCube u_Skybox;

void main()
{
    o_Color = texture(u_Skybox, v_TexCoords);
//    o_Color = vec4(v_TexCoords, 1);
    o_Entity = -1;
}