#include "ConstantBuffer_PostEffect.hlsli"
float4 main(PeraType input) : SV_TARGET{

	float offsetU = 1.0f / 1920.0f*2.0f;
	float offsetV = 1.0f / 1080.0f*2.0f;

	float4 tex_color = tex.Sample(smp, input.uv);

	//ïΩãœÉuÉâÅ[Å@Ç⁄Ç©Çµ
	tex_color += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));
	tex_color += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));
	tex_color += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
	tex_color += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
	tex_color += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
	tex_color += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));
	tex_color += tex.Sample(smp, input.uv + float2(0.0f, offsetV));
	tex_color += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));
	tex_color /= 9.0f;

	return tex_color;
}