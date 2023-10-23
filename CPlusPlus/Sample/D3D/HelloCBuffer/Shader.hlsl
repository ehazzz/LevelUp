cbuffer CBuffer : register(b0)
{
    float4x4 MVP;
    float padding[48];
};


struct Attributes
{
    float4 positionOS : POSITION;
    float4 color : COLOR;
};


struct Varyings
{
    float4 positionCS : SV_POSITION;
    float4 color : COLOR;
};


Varyings VS(Attributes input)
{
    Varyings ret;
    ret.positionCS = mul(MVP, input.positionOS);
    ret.color = input.color;
    
    return ret;
}


float4 PS(Varyings input) : SV_TARGET
{
    //return float4(MVP._11, MVP._12, MVP._13, MVP._14);
    return input.color;
}