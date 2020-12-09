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
    outDiffuse.rgb = max(outDiffuse.rgb, Light.Ambient.rgb * Material.Ambient.rgb * tex.rgb);
    
    //スペキュラ(フォン)
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);
    float3 pos;
    pos.xyz = 10;
    float3 L = normalize(pos - In.WorldPosition.xyz); //ライトベクトル
    
    //float specular = -dot(eyev, refv);
    //specular = saturate(specular);
    //specular = pow(specular, 10);
    
    //outDiffuse.rgb += specular;
    
   
}
