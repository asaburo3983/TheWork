#include "ConstantBuffer_PostEffect.hlsli"

float4 DepthOfField(float2 _uv) {
	float4 tex_color = tex.Sample(smp, _uv);
	float4 tex_blur = texBlur.Sample(smp, _uv);
	float tex_depth = texDepth.Sample(smp,_uv);

	float forcus_depth = texDepth.Sample(smp, float2(forcusU, forcusV));
	float lerpValue = abs(tex_depth - forcus_depth) * 100;
	if (lerpValue > 1.0f) {
		lerpValue = 1.0f;
	}
	if (lerpValue < 0.0f) {
		lerpValue = 0;
	}
	return lerp(tex_color, tex_blur, lerpValue);
}
float4 Sharp(float2 _uv) {

	float offsetU = 1.0f / 1920.0f;
	float offsetV = 1.0f / 1080.0f;

	float power = 2.0f;
	float vicinity = (power - 1.0f) / 4.0f * -1;
	float4 anser = 0;
	float offsetPower = 4.0f;
	anser += tex.Sample(smp, _uv + float2(-offsetPower * offsetU, -offsetPower * offsetV)) * 0;//LU
	anser += tex.Sample(smp, _uv + float2(0, -offsetPower * offsetV)) * vicinity;//U
	anser += tex.Sample(smp, _uv + float2(offsetPower * offsetU, -offsetPower * offsetV)) * 0;//RU

	anser += tex.Sample(smp, _uv + float2(-offsetPower * offsetU, 0.0f)) * vicinity;//L
	anser += tex.Sample(smp, _uv) * power;
	anser += tex.Sample(smp, _uv + float2(offsetPower * offsetU, 0.0f)) * vicinity;//R

	anser += tex.Sample(smp, _uv + float2(-offsetPower * offsetU, offsetPower * offsetV)) * 0;//LD
	anser += tex.Sample(smp, _uv + float2(0, -offsetPower * offsetV)) * vicinity;//D
	anser += tex.Sample(smp, _uv + float2(offsetPower * offsetU, offsetPower * offsetV)) * 0;//RD
	return anser;
}
float4 Outline(float2 _uv ,float4 _baseColor) {
	float offsetU = 1.0f / 1920.0f;
	float offsetV = 1.0f / 1080.0f;

	float tex_depth = texDepth.Sample(smp, _uv);

	
	float4 anser = _baseColor;
	//２ピクセル分のアウトラインを作成
	float depthL = texDepth.Sample(smp, _uv + float2(-offsetU, 0));
	float depthR = texDepth.Sample(smp, _uv + float2(offsetU, 0));
	float depthU = texDepth.Sample(smp, _uv + float2(0, -offsetV));
	float depthD = texDepth.Sample(smp, _uv + float2(0, offsetV));

	float depthL2 = texDepth.Sample(smp, _uv + float2(-offsetU*2, 0));
	float depthR2 = texDepth.Sample(smp, _uv + float2(offsetU*2, 0));
	float depthU2 = texDepth.Sample(smp, _uv + float2(0, -offsetV*2));
	float depthD2 = texDepth.Sample(smp, _uv + float2(0, offsetV*2));

	//float depthL2 = texDepth.Sample(smp, _uv + float2(-offsetU*2, 0));
	//float depthR2 = texDepth.Sample(smp, _uv + float2(offsetU*2, 0));

	float depthAll = 0;
	depthAll = max(depthL, depthR);
	depthAll = max(depthAll, depthU);
	depthAll = max(depthAll, depthD);
	depthAll = max(depthAll, depthR2);
	depthAll = max(depthAll, depthL2);
	depthAll = max(depthAll, depthU2);
	depthAll = max(depthAll, depthD2);

	float depthOutline = abs(depthAll - tex_depth) * 100;
	float3 outlineColor = 0.0f;

	outlineColor = lerp(anser.xyz, outlineColor, depthOutline);
	return float4(outlineColor, 1);

}
float4 main(PeraType input) : SV_TARGET{



	float4 tex_color = tex.Sample(smp, input.uv);
	float4 anser = 1.0f;

	switch (effectNum) {
	case NONE:
		anser = tex_color;
		break;
	case DEPTHOFFIELD:
		anser = DepthOfField(input.uv);
		break;
	case BLUR:
		anser = lerp(tex_color, texBlur.Sample(smp, input.uv), blurPower);
		break;
	case SHARP:
		anser = Sharp(input.uv);
		break;
	case RETORO:
		anser = float4(tex_color.rgb - fmod(tex_color, 1.0f / 4.0f), 1);
		break;
	case NEGA:
		anser = float4(float3(1.0, 1.0, 1.0) - tex_color.rgb, 1);
		break;
	case OUTLINE:
		anser = Outline(input.uv, tex_color);
		break;
	case OUTLINE_DEPTHOFFIELD:
		anser = DepthOfField(input.uv);
		anser = Outline(input.uv,anser);
		break;
	}

	anser *= colorMulti;
	anser += colorPlus;
	return anser;
	

}