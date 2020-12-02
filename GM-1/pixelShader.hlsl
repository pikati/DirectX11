#include "Common.hlsl"

Texture2D		g_texture : register( t0 );
SamplerState	g_samplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in PS_IN In, out float4 outDiffuse		: SV_Target )
{

    float4 normal = normalize(In.Normal);
    
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light) + 0.25;
    
    outDiffuse = g_texture.Sample(g_samplerState, In.TexCoord);
    float4 tex = outDiffuse;
    outDiffuse.rgb *= In.Diffuse.rgb * light * Light.Diffuse.rgb;
    outDiffuse.a *= In.Diffuse.a;
    float3 ambient = Light.Ambient.rgb * Material.Ambient.rgb * tex.rgb;
    outDiffuse.rgb += ambient;
    
    //スペキュラ(フォン)
    //float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    //eyev = normalize(eyev);
    
    //float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    //refv = normalize(refv);
    
    //float specular = -dot(eyev, refv);
    //specular = saturate(specular);
    //specular = pow(specular, 10);
    
   // outDiffuse.rgb += specular;

}
