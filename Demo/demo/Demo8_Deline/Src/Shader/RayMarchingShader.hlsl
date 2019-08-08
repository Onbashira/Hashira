
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

//âEéËç¿ïWånÅ®ç∂éËç¿ïWånÇ…
void mainImage(out PSOut psOut, in VSOut input, in float2 fragCoord);

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

float3 lookAt( float3 p ,float3 eye, float3 target , float3 up)
{

    float3 w = normalize(target - eye), u = normalize(cross(w, up));
    return float3(dot(p, u), dot(p, cross(u, w)), dot(p, w));
}

float dePoint(float3 ro , float3 rd , float3 a)
{
    return (length(cross(a - ro, rd)));

}


float deLine(float3 ro, float3 rd, float3 a, in float3 b)
{
    float3 ab = normalize(b - a), ao = a - ro;
    float d0 = dot(rd, ab), d1 = dot(rd, ao), d2 = dot(ab, ao);
    float len = (d0 * d1 - d2) / (1.0 - d0 * d0);
    len = clamp(len, 0.0, length(b - a));
    float3 p = ab * len + a;
    return length(cross(p - ro, rd));
}

float deCircle(float3 ro, float3 rd, float3 p, float3 n, float r)
{
    float de = 1e10;
    float3 u = normalize(cross(rd, n));
    float3 v = cross(u, n);
    float3 q = dot(p - ro, n) / dot(rd, n) * rd + ro;
    float g = min(r, dot(u, q - p));
    float h = sqrt(r * r - g * g);
    float3 a = u * g + p;
    de = min(de, length(cross(a + v * h - ro, rd)));
    de = min(de, length(cross(a - v * h - ro, rd)));
    a = normalize(q - p) * r + p;
    de = min(de, length(cross(a - ro, rd)));
    a = rd * dot(p - ro, rd) + ro - p;
    a = normalize(cross(cross(n, a), n));
    a = a * r + p;
    de = min(de, length(cross(a - ro, rd)));
    return de;
}

float deBox(float3 ro, float3 rd, float3 size, float3 pos)
{
    float3 c[8];
    for(int i = 0; i < 8; i++)
    {
        float3 p = float3(i >> 2 & 1, i >> 1 & 1, i & 1) * 2.0 - 1.0;
        p *= size;
        float3 u = normalize(float3(pos.xy, 0));
        float3 v = normalize(cross(float3(0, 0, 1), u));
        p = mul(float3x3(v, u, cross(u, v)), p);
        p += pos;
        c[i] = p;
    }
    float de = 1e9;
    for (int j = 0; j < 4; j++)
    {
        de = min(de, deLine(ro, rd, c[(j >> 1 & 1) * 3], c[(j & 1) + 1]));
        de = min(de, deLine(ro, rd, c[(j >> 1 & 1) * 3 + 4], c[(j & 1) + 5]));
        de = min(de, deLine(ro, rd, c[j], c[j + 4]));
    }
    return de;
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

float3 hash(uint i)
{
    uint3 x = uint3(123, 456, 789) * (i + 55u);
    uint k = 1103515245U;
    x *= k;
    x = ((x >> 2u) ^ (x.yzx >> 1u) ^ x.zxy) * k;
    return float3(x) * (1.0 / float(0xffffffffU));
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
{ //ãóó£ä÷êî
    return length(pos) - size;
}

static float ind = 0.0;

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

float SSS(float3 ray, float3 rd, float d , float size)
{
    return clamp(distanceFunction(ray + rd * d, size), 0.0, 1.0);
}

void mainImage(out PSOut psOut, in VSOut input, in float2 fragCoord)
{
    float2 uv = fragCoord;
    float3 ro = float3(0, 1, 0);
    float3 ta = float3(0, 3, -10);
    ta.xz = rot(ta.xz, 0.05);
    float3 rd = lookAt(normalize(float3(uv, 2.0)), ro, ta, normalize(float3(0, 1, 0)));
    float3 col = float3(0 , 0,0);
    float de;
    de = dePoint(ro, rd, float3(0, 0, 50));
    col = lerp(col, float3(0.8, 0.4, 1), 2.0 / de);
    for (uint i = 0u; i < 80u; i++)
    {
        float3 p = hash(i) * 2.0 - 1.0;
        p *= float3(20, 20, 80);
        p += sign(p) * float3(2, 2, 0);
        p.z = mod(p.z + Time * 8.0, 160.0) - 80.0;
        de = deBox(ro, rd, (hash(i + 136u) * 2.0 - 1.0) + float3(2, 1, 3), p);
        float3 b_col = lerp(float3(0.2, 0.4, 1), float3(0.3, 0.5, 0.9), step(0.0, p.z));
        col = lerp(col, b_col, 0.05 / de);
    }
    for (float j = 0.0; j < 10.0; j++)
    {
        de = deCircle(ro, rd, float3(0, 0, j * 3.0 - 5.0), float3(0, 0, 1), 8.0);
        col = lerp(col, float3(0.3, 0.8, 0.4), 0.05 / de);
    }
    psOut.color = float4(col, 1.0f);
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
    float2 p = (input.position.xy * 2.0 - Resolution) / min(Resolution.x, Resolution.y);

    mainImage(output , input, p.xy);

    return output;
}