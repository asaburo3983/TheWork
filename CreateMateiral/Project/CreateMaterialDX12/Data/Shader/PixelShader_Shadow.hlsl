#include "ConstantBuffer.hlsli"

float4 main(VS_OUT input) : SV_TARGET{
    float4 tex_color = tex.Sample(smp, input.uv);
    if (tex_color.a <= 0.0)discard;//a‚O’l”ñ•\Ž¦ˆ—

    return tex_color;
}