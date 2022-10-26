Texture2D MainTexture;
SamplerState MainTextureSampler;

struct Light
{
    float3 position;
    float3 lightDirection;
    float lightIntensity;
    float4 ambientLight;
    float4 diffuseLight;
    float4 color;
};

cbuffer PixelShaderBuffer
{
    Light light;
};

float4 PS(float4 position : SV_POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL) : SV_TARGET
{
    float4 tex = MainTexture.Sample(MainTextureSampler, uv); // texture color
    
    float3 normLight = normalize(-light.lightDirection);    // normalized light direction from point towards light source
    normal = normalize(normal);                             // normalize normal because of interpolation
    float diff = dot(normLight, normal);                    // calculate light intensity in relation to normal & light direction
    diff = max(diff, 0.0f);                                 // dot product can be negative, but there is no negative light
    diff *= light.lightIntensity;                           // light intensity multiplicator
    
    return tex * (light.ambientLight + light.diffuseLight * diff);
}