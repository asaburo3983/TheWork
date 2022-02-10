
//ペラポリゴン描画用
struct PeraType {
	float4 pos: SV_POSITION;
	float2 uv:TEXCOORD;
};

Texture2D<float4> tex : register(t0);
Texture2D<float> texDepth : register(t1);

SamplerState smp : register(s0);

float4 PeraPS(PeraType input) : SV_TARGET{

	float offsetU = 1.5f / 1280.0f;
	float offsetV = 1.5f / 720.0f;

	float4 tex_color = tex.Sample(smp, input.uv);

	//平均ブラー　ぼかし
	if (0) {
		tex_color += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));
		tex_color += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));
		tex_color += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
		tex_color += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
		tex_color += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
		tex_color += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));
		tex_color += tex.Sample(smp, input.uv + float2(0.0f, offsetV));
		tex_color += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));
		tex_color /= 9.0f;
	}

	//色反転
	if (0) {
		return float4(float3(1.0, 1.0, 1.0) - tex_color.rgb, 1);
	}

	//色調を減らす　レトロ
	if (0) {
		return float4(tex_color.rgb - fmod(tex_color.rgb, 1.0f/2.0f) , 1);
	}

	return tex_color;
}