
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

static float Saw = 2.0 * Time - floor(2.0 * Time);;

//右手座標系→左手座標系に

float mod(float x, float y)
{
    return x - y * floor(x / y);
}
float2 mod(float2 x, float2 y)
{
    return x - y * floor(x / y);
}
float3 mod(float3 x, float3 y)
{
    return x - y * floor(x / y);
}
float4 mod(float4 x, float4 y)
{
    return x - y * floor(x / y);
}

float3 hsv(float h, float s, float v)
{
    float4 t = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(float3(h, h, h) + t.xyz) * float3(6.0, 6.0, 6.0) - float3(t.w, t.w, t.w));
    return v * lerp(float3(t.x, t.x, t.x), clamp(p - float3(t.x, t.x, t.x), 0.0, 1.0), s);

}

float2x2 rot(float a)
{
    float s = sin(a);
    float c = cos(a);
    return float2x2(c, -s, s, c);
}

float2 rot(float2 p , float a )
{
    return float2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));

}

float3 trans(float3 p, float len)
{
    return mod(p, len) - (len * 0.5);
}

float2 trans(float2 p, float len)
{
    return mod(p, len) - (len * 0.5);
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
    return mul(p, m);
}

float sdBar(float2 p, float width)
{
    return length(max(abs(p) - width, 0.0));
}

float sdTube(float2 p, float width)
{
    return length(p) - width;
}

float sdSphere(float3 p, float sphereSize)
{
    p = rotate(p + float3(p.x, p.y, p.z), radians(Time * 10.0f), float3(0.0, 0.0, 1.0));
    return (length(trans(p, 2.0)) - sphereSize);
}

float sdBox(float3 p, float3 boxSize)
{
    float3 pp = trans(p, 2.0);
    pp = float3(mul(pp.xy, rot(Time + sin(Time + p.z))), pp.z);
    float3 q = abs(pp);
    return length(max(q - boxSize, 0.0)) - 0.1;
}

float sdBox(float3 p, float boxSize)
{
    float3 pp = trans(p, 2.0);
    pp = float3(mul(pp.xy, rot(Time + sin(Time + p.z))), pp.x);
    float3 q = abs(pp);
    return length(max(q - float3(boxSize, boxSize, boxSize), 0.0)) - 0.1;
}

float distanceFunction(float3 pos, float size)
{ //距離関数
    return length(pos) - size;
}

static float ind = 0.0;
static float Size = Saw * 0.5 + 1.2;

float map ( float3 pos , float size)
{
    ind = floor(5.0 * ceil(0.05 * pos.z) + 1.5 * ceil(0.05 * pos.x) * 1.0 * ceil(5 + 0.05 * pos.y));
    return distanceFunction(mod(pos, 10.0) -5.0, size);

}

float3 ScemeNormal(float3 p , float size)
{
    float2 e = float2(NORMAL_EPS, 0.0);
    float d = map(p, size);
    float3 n = float3(d, d, d) - float3(
        map( p - e.xyy, size),
        map( p - e.yxy, size),
        map( p - e.yyx, size)
    );

    return normalize(n);

}

float SSS(float3 ray, float3 rd, float d)
{
    return clamp(distanceFunction(ray + rd * d, Size), 0.0, 1.0);
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
    //Screen pos
    float2 p = (input.position.xy * 2.0 - Resolution) / min(Resolution.x, Resolution.y);
    //float2 p = input.texcoord;

    float ht = 0.5 * Time;
    float3 ta = float3(0.5, 0.5, 3.0 + 24.0 * sin(ht));

	//Camera
    float3 cameraPos = float3(0 + sin(Time), 3.5 * sin(ht), -3.0 + 24.0 * sin(ht));
    cameraPos.xz = rot(cameraPos.xz, Time);
    
    float3 cameraDir = float3(0.0, 0.0, 1.0);
    float3 fwd = normalize(ta - cameraPos);
    float3 cameraUpward = float3(0.0, 1.0, 0.0);
    float3 side = normalize(cross(fwd, cameraUpward));
    cameraUpward = normalize(cross(side, fwd));
    float screenZ = 1.8; //スクリーンポジション
    float3 RayDir = normalize(p.x * side + p.y * cameraUpward + fwd * screenZ); //レイの方向
    
	// marching loop
    float dist = 0.0;
    float rLen = 0.0;
    float3 color = float3(0.0, 0.0, 0.0);
    float3 SunLight = normalize(float3(-1.0, 1.0, 1.0));
    float ac = 0.0;

    float3 lightDir; //ライトの位置
    
    float depth = 0.0; //レイの進んだ距離
    float3 rayPos = float3(0.0, 0.0, 0.0); //レイの位置
    float3 col = float3(0.0, 0.0, 0.0);

    int j = 1;
    for (int i = 0; i < StepCount; i++)
    {
        rayPos = cameraPos + (RayDir * depth); //レイの位置を計算
        dist = map(rayPos, Size); //一番近いオブジェクトまでの距離を測る
        if (dist <EPS)
            break; //オブジェクトに例がぶつかったらfor文を抜ける
        depth += dist; //レイの進んだ距離の更新
        j = i + 1;
    }

    lightDir = normalize(-rayPos + cameraPos);

    col =  hsv(ind * 0.3, 1.0, 1.0);

    float diff = 0.0f;
    if(diff  <EPS)
    {
        float3 n = ScemeNormal(rayPos, 1.0);
        diff = max(dot(n, lightDir), 0.01);
        
    }

    col *= diff;

    col += float3(float(j) / 128, float(j + 16) / 128, float(j) / 32);
    col += SSS(rayPos, RayDir, 0.05) * 0.005 + SSS(rayPos, RayDir, 0.025) * 0.0025;

    output.color = float4(col, 1.0f);
    return output;
}