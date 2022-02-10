#include "ConstantBuffer.hlsli"

float4 main(VS_OUT input) : SV_TARGET{

    //テクスチャカラー
    float4 tex_color = tex.Sample(smp, input.uv);
    if (tex_color.a <= 0.0)discard;//a０値非表示処理
    return tex_color;
}