
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
float2x2 rot(float a)
{
    float s = sin(a);
    float c = cos(a);
    return float2x2(c, -s, s, c);
}
float sdBox(float3 p, float3 r)
{
    p = abs(p) - r;
    return max(max(p.x, p.y), p.z);
}

float3 rep(float3 p, float r)
{
    float3 re = fmod(p, r) - 0.5 * r;
    return re;
}

float ifsBox(float3 p)
{
    for (int i = 0; i < 3; i++)
    {
        p = abs(p) - 2.;
        p.xy = mul(p.xy,rot(0.3));
        p.yz = mul(p.yz,rot(sin(Time)));
    }
    return sdBox(p, float3(0.3, 0.5, 0.3));
}

float map(float3 p)
{
    float d = ifsBox(rep(p, 8.));
    return d;
}

float3 hsv(float h, float s, float v)
{
    return ((clamp(abs(frac(float3(h, h, h) + float3(0, 2, 1) / 3.) * 6. - 3.) - 1., 0., 1.) - 1.) * s + 1.) * v;
}

float3 trans(float3 p, float len)
{
    return fmod(p, len) - (len * 0.5);
}

float sdSphere(float3 p, float sphereSize)
{
    return (length(trans(p, 2.0)) - sphereSize);
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
    float acc = 0.;



    for (int i = 0; i < StepCount; i++)
    {
        float3 rayPos = cameraPos + ray * dist;
        float dest = map(rayPos);

        dest = max(abs(dest), 0.02);

        float a = exp(-dest * 5.0);
        a *= 2.;
        if (fmod(rayPos.z - 90.0 * Time, 50.0) < sin(Time - floor(Time)))
        {
            a *= 4.0;
        }

        acc += a;

        dist += dest;
    }

   // color = float3(ac * 0.01 * saw * cos(Time) * 0.1, ac * 0.02 * sin(tTime  * saw), ac * sin(saw) * 0.1);
    color = hsv(frac(-0.06 * Time), 0.6, acc * 0.01);

    output.color = float4(color, 1.0f);
    return output;
}
