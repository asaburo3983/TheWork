Texture2D<float4> tex : register(t1);
Texture2D<float> texDepth : register(t2);
Texture2D<float4> texBlur : register(t3);

SamplerState smp : register(s0);

static const int NONE = 0;
static const int DEPTHOFFIELD = 1;
static const int BLUR = 2;
static const int SHARP = 3;
static const int RETORO = 4;
static const int NEGA = 5;
static const int OUTLINE = 6;
static const int OUTLINE_DEPTHOFFIELD = 7;

cbuffer PostEffectBuffer : register(b0) {
	float forcusU;
	float forcusV;
	int effectNum;
	float blurPower;
	float4 colorPlus;
	float4 colorMulti;

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