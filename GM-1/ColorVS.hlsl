
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

void main(in float4 inPosition : POSITION0,
						  in float4 inDiffuse : COLOR0,

						  out float4 outPosition : SV_POSITION,
						  out float4 outDiffuse : COLOR0)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    outPosition = mul(inPosition, wvp);
	
    float4 worldNormal, normal;
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    outDiffuse = inDiffuse;

}

