#define PostEffectTestRS   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                    "DescriptorTable(CBV(b0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "StaticSampler(s0 ,"\
                                             "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                             "addressU = TEXTURE_ADDRESS_WRAP,"\
                                             "addressV = TEXTURE_ADDRESS_WRAP,"\
                                             "addressW = TEXTURE_ADDRESS_WRAP,"\
                                             "mipLodBias = 0.0f,"\
                                             "maxAnisotropy = 16,"\
                                             "comparisonFunc  = COMPARISON_NEVER,"\
                                             "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                             "minLOD = 0.0f,"\
                                             "maxLOD = 3.402823466e+38f,"\
                                             "space  =  0,"\
                                             "visibility = SHADER_VISIBILITY_ALL"\
                                            ")"\





#define F_PI  3.141592f;

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 svPos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 color : Sv_Target0;
};

typedef PS_INPUT VS_OUTPUT;

cbuffer Material : register(b0)
{
   
    float4 MatColor;
};

[RootSignature(PostEffectTestRS)]
VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.svPos = float4(input.pos, 1.0f);
    output.texcoord = input.texcoord;
    return output;
}

PS_OUTPUT PSMain(PS_INPUT input)
{

    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.color = MatColor;
    return output;

}