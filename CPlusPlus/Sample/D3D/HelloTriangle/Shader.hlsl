

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
    ret.positionCS = input.positionOS;
    ret.color = input.color;
    
    return ret;
}

float4 PS(Varyings input) : SV_TARGET
{
    return input.color;
}