//GLSL version
#version 330 core
const int MAX_POINT_LIGHTS  =   6;
const int MAX_SPOT_LIGHTS   =   2;
//Input

in vec2 CoordTexture;
in vec3 Normal0;
in vec3 WorldPos0;

// Positional Light
struct BaseLight
{
    vec3    Color;
    float   AmbientIntensity;
    float   DiffuseIntensity;
};
struct DirectionalLight
{
    BaseLight   Base;
    vec3        Direction;
};
struct  Attenuation
{
    float   Constant;
    float   Linear;
    float   Exp;
};
struct  PointLight
{
    BaseLight   Base;
    vec3        Position;
    Attenuation Atten;
};
struct  SpotLight
{
    PointLight  Base;
    vec3        Direction;
    float       Cutoff;
};

uniform int                 NumPointLights;
uniform int                 NumSpotLights;
uniform PointLight          PointLights[MAX_POINT_LIGHTS];
uniform SpotLight           SpotLights[MAX_SPOT_LIGHTS];
uniform DirectionalLight    directionalLight;
uniform vec3                eyeWorldPos;
uniform float               matSpecularIntensity;
uniform float               matSpecularPower;
// dont forget sampler2D text
uniform sampler2D               text;
// Output
layout (location = 0) out vec4 out_Color; // for 330 and more
// Function declaration
vec4    CalcLightInternal(in BaseLight Light, in vec3 LightDirection,in vec3 Normal);
vec4    CalcDirectionalLight(in vec3 Normal);
vec4    CalcPointLight(in PointLight pl, in vec3 Normal);
vec4    CalcSpotLight(in SpotLight  sl, in vec3 Normal);

// Implementation
vec4    CalcLightInternal(in BaseLight Light, in vec3 LightDirection,in vec3 Normal)
{

    // Ambient color
    vec4    ambientColor    =   vec4(Light.Color,1.0f)*Light.AmbientIntensity;
    // diffuse factor
    float diffuseFactor     =   dot(Normal, -LightDirection);
    vec4 diffuseColor       =   vec4(0,0,0,0);
    vec4 specularColor      =   vec4(0,0,0,0);
    if (diffuseFactor > 0)
    {
        diffuseColor = vec4(Light.Color, 1.0f)* Light.DiffuseIntensity*diffuseFactor;
        vec3    VertexToEye     =   normalize(eyeWorldPos - WorldPos0);
        vec3    LightReflect    =   normalize(reflect(LightDirection, Normal));
        float   SpecularFactor  =   dot (VertexToEye, LightReflect);
        SpecularFactor          =   pow(SpecularFactor,matSpecularPower);
        if (SpecularFactor > 0)
        {
            specularColor   =   vec4(Light.Color,1.0f)*matSpecularIntensity*SpecularFactor;
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}
vec4    CalcDirectionalLight(in vec3 Normal)
{
    return CalcLightInternal(directionalLight.Base, directionalLight.Direction, Normal);
}
vec4    CalcPointLight(in PointLight pl, in vec3 Normal)
{
    vec3    LightDirection  =   WorldPos0 - pl.Position;
    float   Distance        =   length(LightDirection);
    LightDirection          =   normalize(LightDirection);
    vec4    Color           =   CalcLightInternal(pl.Base, LightDirection, Normal);
    float   atten           =   pl.Atten.Constant +
                                pl.Atten.Linear * Distance +
                                pl.Atten.Exp * Distance * Distance;
    return  Color/atten;
}
vec4    CalcSpotLight(in SpotLight  sl, in vec3 Normal)
{
    vec3    LightToPixel    =   normalize(WorldPos0 - sl.Base.Position);
    float   SpotFactor      =   dot(LightToPixel, sl.Direction);

    if (SpotFactor > sl.Cutoff)
    {
        vec4    Color   =   CalcPointLight(sl.Base, Normal);
        return  Color * (1.0 - (1.0 - SpotFactor)* 1.0/(1.0 - sl.Cutoff));
    }
    else
    {
        return  vec4(0.0,0.0,0.0,0.0);
    }
}

void main()
{
    vec3    Normal      =   normalize(Normal0);
    vec4    TotalLight  =   vec4(0.0,0.0,0.0,0.0);
    // Directional light part
    TotalLight      +=  CalcDirectionalLight(Normal);
    // point light part
    for (int i = 0; i < NumPointLights  && i < MAX_POINT_LIGHTS; ++i)
        TotalLight +=   CalcPointLight(PointLights[i], Normal);
    // spot light part
    for (int i = 0; (i < NumSpotLights) && (i < MAX_SPOT_LIGHTS); ++i)
        TotalLight  += CalcSpotLight(SpotLights[i], Normal);

    out_Color   =   texture2D(text,CoordTexture.xy)*TotalLight;

}

