#include "common.hlsl"

Texture2D g_texture : register(t0);
SamplerState g_samplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //テクスチャマッピング
    outDiffuse = g_texture.Sample(g_samplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;
}