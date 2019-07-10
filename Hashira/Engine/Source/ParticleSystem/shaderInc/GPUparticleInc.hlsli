struct EmitterIndex
{
    uint index;
};

struct EmitterHeader
{
    uint EmtBinHead;
    uint PtBinHead;
    uint PtIdxHead;
    uint PtSize;
};

struct ParticleHeader
{
    uint Key;
    float Depth;
};

struct EmitterRange
{
    uint AliveBegin;
    uint AliveEnd;
    uint DeadBegin;
    uint DeadEnd;
};

struct VertexData
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};