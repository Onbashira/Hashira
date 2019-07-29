

struct VSOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX_COORD;
};

struct PSOut
{
    float4 color : SV_Target0;
};

//struct SceneConstant 
//{

//    float2 Resolution;
//    float Time;
//};

cbuffer SceneConstant : register(b0)
{
    float2 Resolution;
    float Time;
}

VSOut VS_Main(int index : INDEX)
{

    VSOut output;

    output.position = float4((index % 2) * 2 - 1, ((index / 2) * 2 - 1) * -1, 0, 0);
    output.texcoord = float2(index % 2, index / 2);
    return output;
}

PSOut PS_Main(VSOut input)
{
    PSOut output;
    output.color = float4(input.texcoord.x, input.texcoord.y, 0.0, 0.0);
    return output;
}