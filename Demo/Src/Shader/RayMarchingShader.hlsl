
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

static const float PI = 3.14159265;
static const int StepCount = 64;
static const float EPS = 0.001;
static const float NORMAL_EPS = 0.0001;


//âEéËç¿ïWånÅ®ç∂éËç¿ïWånÇ…


float3 hsv(float h, float s, float v)
{
    float4 t = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(float3(h,h,h) + t.xyz) * float3(6.0, 6.0, 6.0) - float3(t.w, t.w, t.w));
    return v * lerp(float3(t.x, t.x, t.x), clamp(p - float3(t.x, t.x, t.x) , 0.0,1.0), s);

}

float3 trans(float3 p, float len)
{
    return fmod(p, len) - (len * 0.5);
}

float sdSphere(float3 p, float sphereSize)
{
    return (length(trans(p, 2.0)) - sphereSize);
}

float2x2 rot(float a)
{
    float s = sin(a);
    float c = cos(a);
    return float2x2(c, -s, s, c);
}

float sdBox(float3 p, float boxSize)
{
    float3 pp = trans(p, 2.0);
    pp = float3(mul(pp.xy, rot(Time + sin(Time + p.z))), pp.z);
    float3 q = abs(pp);
    return length(max(q - float3(boxSize, boxSize, boxSize), 0.0)) - 0.1;
}

float3 getSceneNormal(float3 p, float objectSize)
{
    return normalize(float3(
		sdBox(p + float3(NORMAL_EPS, 0.0, 0.0), objectSize) - sdBox(p + float3(-NORMAL_EPS, 0.0, 0.0), objectSize),
		sdBox(p + float3(0.0, NORMAL_EPS, 0.0), objectSize) - sdBox(p + float3(0.0, -NORMAL_EPS, 0.0), objectSize),
		sdBox(p + float3(0.0, 0.0, NORMAL_EPS), objectSize) - sdBox(p + float3(0.0, 0.0, -NORMAL_EPS), objectSize)
	));
}

VSOut VS_Main(VSInput input)
{

    VSOut output;
    output.position = float4(input.position,1.0f);
    output.texcoord = input.texcoord;

    return output;
}

PSOut PS_Main(VSOut input)
{
    PSOut output;
    float temp = 1.0;
    float saw = 2.0 * Time - floor(2.0 * Time);
    float tTime = 1.0 - 0.3 * modf(Time * 2.0, saw);

    //Screen pos
    float2 p = (input.position.xy * 2.0 - Resolution) / min(Resolution.x, Resolution.y);
    //float2 p = input.texcoord;

	//Camera
    float3 cameraPos = float3(Resolution.x , Resolution.y, 0);
    float3 cameraDir = float3(0.0, 0.0, 1.0);
    float3 cameraUpward = float3(0.0, 1.0, 0.0);
    float3 cameraRightward = cross(cameraDir, cameraUpward);
    float cameraAngle = 60.0 ;
    float cameraFov = cameraAngle * 0.5 * PI / 180.0;

	//ray
    float3 ray = normalize(float3(sin(cameraFov) * p.x, sin(cameraFov) * p.y, cos(cameraFov)));


	// marching loop
    float dist = 0.0;
    float rLen = 0.0;
    float3 rayPos = cameraPos;
    float3 color = float3(0.0,0.0,0.0);
    float3 SunLight = normalize(float3(-1.0, 1.0, 1.0));
    
    float steps = 0.1;
    float boxSize = tTime - 0.6f;
    float ac = 0.0;
    for (int i = 0; i < StepCount; i++)
    {
        dist = sdBox(rayPos, boxSize);
        dist = max(abs(dist),0.02);
        ac += exp(-dist * 10.0);
		
        rLen += dist * 0.5;
        rayPos = cameraPos + ray * rLen;
    }

    color = float3(ac * 0.01 * saw * cos(Time) * 0.1, ac * 0.02 * sin(tTime  * saw), ac * sin(saw) * 0.1);

    output.color = float4(color, 1.0f);
    return output;
}