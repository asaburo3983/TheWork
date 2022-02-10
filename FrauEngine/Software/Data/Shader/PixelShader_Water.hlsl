#include "ConstantBuffer.hlsli"

float4 main(VS_OUT input) : SV_TARGET{
    //UV移動で水の流れを表現
    //テクスチャカラー
    float t = time%600 / 600.0f;
    float u = cos(input.uv.y*2);
    float4 tex_color = tex.Sample(smp, input.uv + float2(u, t));
    if (tex_color.a <= 0.0)discard;//a０値非表示処理
    tex_color.a = 0.3f;

    return tex_color;
}