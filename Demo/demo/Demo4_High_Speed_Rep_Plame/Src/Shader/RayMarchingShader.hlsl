
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


//�E����W�n��������W�n��


float3 hsv(float h, float s, float v)
{
    float4 t = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(float3(h, h, h) + t.xyz) * float3(6.0, 6.0, 6.0) - float3(t.w, t.w, t.w));
    return v * lerp(float3(t.x, t.x, t.x), clamp(p - float3(t.x, t.x, t.x), 0.0, 1.0), s);

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

float3 rotate(float3 p, float angle, float3 axis)
{
    float3 a = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0 - c;
    float3x3 m = float3x3(
        a.x * a.x * r + c,
        a.y * a.x * r + a.z * s,
        a.z * a.x * r - a.y * s,
        a.x * a.y * r - a.z * s,
        a.y * a.y * r + c,
        a.z * a.y * r + a.x * s,
        a.x * a.z * r + a.y * s,
        a.y * a.z * r - a.x * s,
        a.z * a.z * r + c
    );
    return mul(p,m);
}

float sdBox(float3 p, float boxSize)
{
    float3 pp = trans(p, 2.0);
    pp = float3(mul(pp.xy, rot(Time + sin(Time + p.z))), pp.z);
    float3 q = abs(pp);
    return length(max(q - float3(boxSize, boxSize, boxSize), 0.0)) - 0.1;
}

float sdBox(float3 p, float3 r)
{
    p = abs(p) - r;
    return max(max(p.x, p.y), p.z);
}

float3 getSceneNormal(float3 p, float objectSize)
{
    return normalize(float3(
		sdBox(p + float3(NORMAL_EPS, 0.0, 0.0), objectSize) - sdBox(p + float3(-NORMAL_EPS, 0.0, 0.0), objectSize),
		sdBox(p + float3(0.0, NORMAL_EPS, 0.0), objectSize) - sdBox(p + float3(0.0, -NORMAL_EPS, 0.0), objectSize),
		sdBox(p + float3(0.0, 0.0, NORMAL_EPS), objectSize) - sdBox(p + float3(0.0, 0.0, -NORMAL_EPS), objectSize)
	));
}

float ifsBox(float3 p)
{
    for (int i = 0; i < 3; i++)
    {
        p = abs(p) - 2.;
        p.xy = mul(p.xy, rot(0.3));
        p.yz = mul(p.yz, rot(sin(Time)));
    }
    return sdBox(p, float3(0.3, 0.5, 0.3));
}

float map(float3 p)
{
    float saw = 2.0 * Time - floor(2.0 * Time);
    float d1 = ifsBox(trans(p, 8.));
    float d2 = sdBox(p, 0.3 * saw);
    return min(d1, d2);

}

VSOut VS_Main(VSInput input)
{


    VSOut output;
    output.position = float4(input.position, 1.0f);
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

    float fovValue = cos(sin(Time * 2.0) * saw);

	//Camera
    float3 cameraPos = float3(Resolution.x, Resolution.y, 0);
    float3 cameraDir = float3(0.0, 0.0, 1.0);
    float3 cameraUpward = float3(0.0, 1.0, 0.0);
    float3 cameraRightward = cross(cameraDir, cameraUpward);
    float cameraAngle = 60.0;
    float cameraFov = cameraAngle * 0.5 * PI / 180.0;

	//ray
    float3 ray = normalize(float3(sin(cameraFov) * p.x, sin(cameraFov) * p.y, cos(cameraFov)));


	// marching loop
    float dist = 0.0;
    float rLen = 0.0;
    float3 rayPos = cameraPos;
    float3 color = float3(0.0, 0.0, 0.0);
    float3 SunLight = normalize(float3(-1.0, 1.0, 1.0));
    float ac = 0.0;
    for (int i = 0; i < StepCount; i++)
    {
        dist = map(rotate(rayPos, radians(Time * 10), float3(0.0, 1.0, 0.0)));
        dist = max(abs(dist), 0.02);
        ac += exp(-dist * 10.0);
		
        rLen += dist * 0.5;
        rayPos = cameraPos + ray * rLen;
    }

    color = float3(ac * 0.01 * saw * 0.1, ac * 0.02 * sin(saw * tTime), ac * 0.01 * cos(2.0* tTime));

    output.color = float4(color, 1.0f);
    return output;
}