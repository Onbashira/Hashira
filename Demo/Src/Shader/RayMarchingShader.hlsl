
struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEX_COORD;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX_COORD;
};

struct PSOut
{
    float4 color : SV_Target0;
};

cbuffer SceneConstant : register(b0)
{
    float2 Resolution;
    float Time;
}

VSOut VS_Main(VSInput input)
{

    VSOut output;

    output.position = float4(input.position, 0.0f);
    output.texcoord = input.texcoord;

    return output;
}

PSOut PS_Main(VSOut input)
{
    PSOut output;
    output.color = float4(input.texcoord.x + Time, input.texcoord.y + Time, 0.0f, 0.0f);
    return output;
}