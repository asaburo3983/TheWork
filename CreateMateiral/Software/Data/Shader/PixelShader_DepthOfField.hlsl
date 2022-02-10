#include "ConstantBuffer_PostEffect.hlsli"

float4 main(PeraType input) : SV_TARGET{

	float4 tex_color = tex.Sample(smp, input.uv);
	float4 tex_blur = texBlur.Sample(smp, input.uv);
	
	float tex_depth = texDepth.Sample(smp, input.uv);
	float forcus_depth = texDepth.Sample(smp, float2(forcusU, forcusV));

	float lerpValue = abs(tex_depth - forcus_depth);
	float4 anser = lerp(tex_color,tex_blur, lerpValue*100);

	return anser;

}