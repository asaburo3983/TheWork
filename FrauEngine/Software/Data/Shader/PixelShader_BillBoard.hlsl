#include "ConstantBuffer.hlsli"

float4 main(VS_OUT input) : SV_TARGET{

    //�e�N�X�`���J���[
    float4 tex_color = tex.Sample(smp, input.uv);
    if (tex_color.a <= 0.0)discard;//a�O�l��\������
    return tex_color;
}