#include "Common.hlsl"

Texture2D g_texture : register(t0);
Texture2D g_texture2 : register(t1);
SamplerState g_samplerState : register(s0);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    float4 normal = normalize(In.Normal);
    
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light) + 0.25;
    
    outDiffuse = g_texture.Sample(g_samplerState, In.TexCoord) * Paramater.x + g_texture2.Sample(g_samplerState, In.TexCoord) * (1 - Paramater.x);
    float4 tex = outDiffuse;
    outDiffuse.rgb *= In.Diffuse.rgb * light * Light.Diffuse.rgb;
    outDiffuse.a *= In.Diffuse.a;
}
