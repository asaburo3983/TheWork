Texture2D<float4> tex : register(t1);
Texture2D<float> texDepth : register(t2);
Texture2D<float4> texBlur : register(t3);

SamplerState smp : register(s0);

cbuffer PostEffectBuffer : register(b0) {
	float forcusU;
	float forcusV;
};

struct VS_IN {
	float4 pos:POSITION;
	float2 uv : TEXCOORD;

};

//ƒyƒ‰ƒ|ƒŠƒSƒ“•`‰æ—p
struct PeraType {
	float4 pos: SV_POSITION;
	float2 uv:TEXCOORD;
};