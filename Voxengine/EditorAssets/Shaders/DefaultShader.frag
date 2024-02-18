#version 450 core

#define MAX_LIGHT_COUNT 20
#define EPSILON 0.1

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
    vec4 EmissiveFactor;
    int AlphaMode;
    float AlphaCutoff;
    int DoubleSided;
};

struct Light
{
    vec4 Color;
    vec4 Position;
    vec4 Direction;
    float Range;
    float Intensity;
    float Cutoff;
    int Type; //0 = Directional ; 1 = Point ; 2 = Spot
};

struct LightData
{
    Light lights[MAX_LIGHT_COUNT];
//    Light lights;
    int lightCount;
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

layout(std140, binding = 2) uniform Lights
{
    LightData lights;
};

layout(std140, binding = 3) uniform MaterialParameters
{
    MaterialParams materialParameters;
};

layout (location = 0) out vec4 o_Color;
layout (location = 1) out int o_Entity; // -1 = no entity. (for now.)

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoord;
layout (location = 3) in vec4 v_Color;
layout (location = 4) in flat int v_EntityId;

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

vec3 CalculateLighting(vec3 lightColor, vec3 lightPos, vec3 lightDir, float lightIntensity)
{
    // ambient
    float ambientStrength = 0.05 * lightIntensity;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    float diff = max(dot(v_Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    //        // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPosition.xyz - v_Position);
    vec3 reflectDir = normalize(reflect(-lightDir, v_Normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * lightIntensity;

    return (ambient + diffuse + specular) * o_Color.rgb;
}

vec3 AddDirectionalLight(Light light)
{
    vec3 lightColor = light.Color.rgb;
    vec3 lightPos = light.Position.xyz;
    vec3 lightDir = normalize(light.Direction.xyz);
    float lightIntensity = light.Intensity * sign(dot(v_Normal, lightDir));

    return CalculateLighting(lightColor, lightPos, lightDir, lightIntensity);
}

vec3 AddPointLight(Light light)
{
    vec3 lightColor = light.Color.rgb;
    vec3 lightPos = light.Position.xyz;
    vec3 lightDir = normalize(lightPos - v_Position);
    float lightIntensity = light.Intensity * sign(dot(v_Normal, lightDir));

    float distance = distance(v_Position, lightPos);
    float lightIntensity = light.Intensity * sign(dot(v_Normal, lightDir));
    lightIntensity =  lightIntensity / ((1) + (0.09) * distance + (0.032) * (distance * distance));
    lightIntensity = lightIntensity * clamp((1-(distance / light.Range)) * 10, 0., 1.);


    return CalculateLighting(lightColor, lightPos, lightDir, lightIntensity);
}

vec3 AddSpotLight(Light light)
{
    vec3 lightColor = light.Color.rgb;
    vec3 lightPos = light.Position.xyz;
    vec3 lightDir = normalize(lightPos - v_Position);

    if(dot(normalize(lightPos - v_Position) , normalize(-light.Direction.xyz)) <= cos(light.Cutoff))
    {
        return vec3(0);
    }

    float distance = distance(v_Position, lightPos);
    float lightIntensity = light.Intensity * sign(dot(v_Normal, lightDir));
    lightIntensity =  lightIntensity / ((1) + (0.09) * distance + (0.032) * (distance * distance));
    lightIntensity = lightIntensity * clamp((1-(distance / light.Range)) * 10, 0., 1.);

    return CalculateLighting(lightColor, lightPos, lightDir, lightIntensity);
}

void main()
{
    o_Color = materialParameters.PbrMetallicRoughness.BaseColorFactor;

    if(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index > -1)
    {
        o_Color *= SampleTexture(materialParameters.PbrMetallicRoughness.BaseColorTexture.Index);
        if(materialParameters.PbrMetallicRoughness.BaseColorTexture.TexCoord > 0)
        {
            o_Color = vec4(0.8, 0.2, 0.3, 1.0);
        }
    }

    vec3 result = vec3(0);

    for(int i = 0; i < int(lights.lightCount); i++)
    {
        Light light = lights.lights[i];

        if(light.Type == 0)
        {
            result += AddDirectionalLight(light);
        }
        else if(light.Type == 1)
        {
            result += AddPointLight(light);
        }
        else if(light.Type == 2)
        {
            result += AddSpotLight(light);
        }
    }

    o_Color = vec4(result, 1);
    o_Entity = v_EntityId;
}
