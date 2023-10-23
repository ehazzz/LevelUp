Texture2D Sample : register(t0);
SamplerState SampleSampler : register(s0);


struct Attributes
{
    float4 positionOS : POSITION;
    float2 uv : TEXCOORD;
};


struct Varyings
{
    float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD;
};


Varyings VS(Attributes input)
{
    Varyings ret;
    ret.positionCS = input.positionOS;
    ret.uv = input.uv;
    
    return ret;
}


float4 PS(Varyings input) : SV_TARGET
{
    //return float4(input.uv, 0, 1);
    return Sample.Sample(SampleSampler, input.uv);
}