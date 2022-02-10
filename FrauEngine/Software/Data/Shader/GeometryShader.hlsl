


cbuffer WorldBuffer : register(b0) {
	matrix World;
}
cbuffer BoneBuffer : register(b1) {
	matrix mBoneMatrix[512];
}
cbuffer MaterialBuffer : register(b2) {
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
}
cbuffer CameraBuffer : register(b3) {
	matrix CameraView;
	matrix CameraProjection;
	float4 CameraPos;
}
cbuffer DirectionalLightBuffer : register(b4) {
	matrix DLightView;
	matrix DLightProjection;
	float4 DLightPos;

	float4	DLightVector;
	float4	DLightColor;
}

cbuffer UtilityBuffer : register(b5) {
	int shadow;
}

cbuffer PBRBuffer : register(b6) {
	int bmpTexture;
	int anotherTexture0;
	int anotherTexture1;
	int anotherTexture2;

	float subsurface;

	float metallic;

	float specular;

	float specularTint;

	float roufhness;

	float anistropics;

	float sheen;

	float sheenTint;

	float clearColor;

	float clearcoatGloss;
}


struct VS_OUT {
	float4 pos : SV_POSITION;//システム用頂点座標
	float4 worldPos : POSITION;
	float4 normal : NORMAL;//法線ベクトル
	float2 uv  : TEXCOORD;//UV値
	float3 posSM : POSITION_SM;//システム用頂点座標

	float4 posDLight : POSITION1;
	float4 normalDLight : NORMAL1;
};
struct GS_OUT {
	float4 pos : SV_POSITION;//システム用頂点座標
	float4 worldPos : POSITION;
	float4 normal : NORMAL;//法線ベクトル
	float2 uv  : TEXCOORD;//UV値
	float3 posSM : POSITION_SM;//システム用頂点座標

	float4 posDLight : POSITION1;
	float4 normalDLight : NORMAL1;

};

// ジオメトリ シェーダの関数
[maxvertexcount(3)]
void main(triangle VS_OUT input[3],
	inout TriangleStream<GS_OUT> TriStream) {
	GS_OUT output;

	// 各頂点の計算
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.worldPos = input[i].worldPos;
		output.normal = input[i].normal;

		output.uv = input[i].uv;

		output.posSM = input[i].posSM;
		output.posDLight = input[i].posDLight;
		output.normalDLight = input[i].normalDLight;

		// 出力
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}