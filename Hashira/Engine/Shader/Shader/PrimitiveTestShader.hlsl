#define PrimitiveTestRS   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                    "DescriptorTable(CBV(b0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(CBV(b1,numDescriptors = 1,space = 0)," \
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
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 svPos : SV_POSITION;
    float3 rawPos : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 color : Sv_Target0;
};

typedef PS_INPUT VS_OUTPUT;

struct Camera
{
    matrix View;
    matrix Proj;
    matrix ViewInv;
    matrix ProjInv;
    float2 WindowSize;
};

cbuffer CameraInfo : register(b0)
{
    Camera camera;
};

cbuffer Transform : register(b1)
{
   
    matrix transform;
};

[RootSignature(PrimitiveTestRS)]
VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.rawPos = input.pos;
    output.svPos = mul(camera.Proj, mul(camera.View, mul(transform, float4(input.pos, 1.0f))));


        //test
    {
    
        output.rawPos = input.pos;
        output.svPos = mul(camera.Proj, mul(camera.View, float4(input.pos, 1.0f)));
        output.normal = float4(normalize(input.normal), 0.0f);
        output.texcoord = input.texcoord;
        return output;
    }
    //
    matrix rotation = transform;
    matrix scaleInv = 0;
    rotation._m03_m13_m23 = 0;

    float sx = length(float3(transform._11_21_31));;
    float sy = length(float3(transform._12_22_32));;
    float sz = length(float3(transform._13_23_33));;

    scaleInv._11 = rcp(sx);
    scaleInv._22 = rcp(sy);
    scaleInv._33 = rcp(sz);
    scaleInv._44 = 1.0f;
    matrix rot = mul(scaleInv, rotation);

    output.normal = mul(rot, float4(normalize(input.normal), 0.0f));
    output.texcoord = input.texcoord;
    return output;
}

PS_OUTPUT PSMain(PS_INPUT input)
{

    PS_OUTPUT output = (PS_OUTPUT) 0;
    float3 directionalLight = float3(0, -1, 0);
    //test lighting

    float bright = dot(input.normal.xyz, -directionalLight) ;
    float3 ambient = float3(0.2f, 0.2f, 0.2f);
    

    float3 color = float3(bright, bright, bright) + ambient;
    output.color = float4(color, 1.0f);
    return output;

}