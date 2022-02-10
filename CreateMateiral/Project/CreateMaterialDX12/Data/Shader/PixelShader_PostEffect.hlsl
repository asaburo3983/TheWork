#include "ConstantBuffer_PostEffect.hlsli"


float4 main(PeraType input) : SV_TARGET{

	float offsetU = 1.0f / 1920.0f;
	float offsetV = 1.0f / 1080.0f;

	float4 tex_color = tex.Sample(smp, input.uv);
	//���σu���[�@�ڂ���
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
	//�G���{�X���H
	if (0) {
		float4 anser=0;
		anser += tex.Sample(smp, input.uv + float2(-2 * offsetU, -2 * offsetV)) * 2;//LU
		anser += tex.Sample(smp, input.uv + float2(0, -2 * offsetV)) * 2;//U
		anser += tex.Sample(smp, input.uv + float2(2 * offsetU, -2 * offsetV)) * 0;//RU

		anser += tex.Sample(smp, input.uv + float2(-2*offsetU, 0.0f));//L
		anser += tex.Sample(smp, input.uv);
		anser += tex.Sample(smp, input.uv + float2(2*offsetU, 0.0f))*-1;//R

		anser += tex.Sample(smp, input.uv + float2(-2*offsetU, 2 * offsetV))*0;//LD
		anser += tex.Sample(smp, input.uv + float2(0, -2*offsetV));//D
		anser += tex.Sample(smp, input.uv + float2(2*offsetU, 2 * offsetV))*-2;//RD

		return anser;
	}
	//�G�b�W����
	if (0) {
		float power = 5.0f;
		float vicinity = (power - 1.0f) / 4 * -1;
		float4 anser = 0;
		anser += tex.Sample(smp, input.uv + float2(-2 * offsetU, -2 * offsetV)) * 0;//LU
		anser += tex.Sample(smp, input.uv + float2(0, -2 * offsetV)) * vicinity;//U
		anser += tex.Sample(smp, input.uv + float2(2 * offsetU, -2 * offsetV)) * 0;//RU

		anser += tex.Sample(smp, input.uv + float2(-2 * offsetU, 0.0f))* vicinity;//L
		anser += tex.Sample(smp, input.uv)*power;
		anser += tex.Sample(smp, input.uv + float2(2 * offsetU, 0.0f)) * vicinity;//R

		anser += tex.Sample(smp, input.uv + float2(-2 * offsetU, 2 * offsetV)) * 0;//LD
		anser += tex.Sample(smp, input.uv + float2(0, -2 * offsetV))* vicinity;//D
		anser += tex.Sample(smp, input.uv + float2(2 * offsetU, 2 * offsetV)) * 0;//RD

		return anser;
	}
	//�F���]
	if (0) {
		return float4(float3(1.0, 1.0, 1.0) - tex_color.rgb, 1);
	}

	//�F�������炷�@���g��
	if (0) {
		return float4(tex_color.rgb - fmod(tex_color.rgb, 1.0f/4.0f) , 1);
	}
	//�ʓx��������
	if (0) {
		return tex_color*1.1f;
	
	}
	//�F����ς���
	if (0) {
		//�Ԃ�
		tex_color.r*=1.2f;
		tex_color.g*=1.0f;
		tex_color.b*=1.0f;

	}
	return tex_color;
}