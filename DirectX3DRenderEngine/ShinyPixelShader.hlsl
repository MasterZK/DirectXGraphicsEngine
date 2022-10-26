struct Light
{
    float3 position;
    float3 lightDirection;
    float lightIntensity;
    float4 ambientLight;
    float4 diffuseLight;
    float4 color;
};

struct CameraData
{
    float4 cameraPos;
    float4 viewDir;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
    float4 worldPos : TEXCOORD1;
};

cbuffer PixelShaderBuffer
{
    Light light;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 PS(float4 position : SV_POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL, float4 worldPos : TEXCOORD1) : SV_TARGET
{
    float4 cameraPos = (0, 0, -5, 0);

    normal = normalize(normal);

    float4 diffuse = ObjTexture.Sample(ObjSamplerState,uv);
    //float4 diffuse = (0, 100, 200, 1);

    float3 finalColor;

    float3 viewDirection = normalize(cameraPos.xyz - worldPos.xyz);

    finalColor = diffuse * light.ambientLight;
    finalColor += saturate(dot(-light.lightDirection, normal) * light.diffuseLight * diffuse);

    float shinyness = 3;

    float3 specRefl;

    specRefl = (light.diffuseLight.rgb * pow(max(0, dot(reflect(-light.lightDirection, normal),
                viewDirection)), shinyness)) * 0.3f;

    return float4(finalColor + saturate(specRefl), diffuse.a);
}