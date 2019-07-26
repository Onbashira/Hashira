

struct VSOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEX_COORD;
};

//struct SceneConstant 
//{

//    float2 Resolution;
//    float Time;
//};

cbuffer SceneConstant : register(c0)
{
    float2 Resolution;
    float Time;
}

    VSOut VS_Main(
    int index : INDEX) : SV_POSITION
{

    VSOut output;
	
    output.position = float4((index % 2) * 2 - 1, ((index / 2) * 2 - 1) * -1, 0, 0);
    output.texcoord = float2(index % 2, index / 2);
    return output;
}

float4 PS_Main(VSOut input) : SV_COLOR
{
	
    return pos;
}