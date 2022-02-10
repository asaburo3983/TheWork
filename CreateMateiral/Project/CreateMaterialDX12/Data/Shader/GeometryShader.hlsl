


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
	float4 pos : SV_POSITION;//�V�X�e���p���_���W
	float4 worldPos : POSITION;
	float4 normal : NORMAL;//�@���x�N�g��
	float2 uv  : TEXCOORD;//UV�l
	float3 posSM : POSITION_SM;//�V�X�e���p���_���W

	float4 posDLight : POSITION1;
	float4 normalDLight : NORMAL1;
};
struct GS_OUT {
	float4 pos : SV_POSITION;//�V�X�e���p���_���W
	float4 worldPos : POSITION;
	float4 normal : NORMAL;//�@���x�N�g��
	float2 uv  : TEXCOORD;//UV�l
	float3 posSM : POSITION_SM;//�V�X�e���p���_���W

	float4 posDLight : POSITION1;
	float4 normalDLight : NORMAL1;

};

// �W�I���g�� �V�F�[�_�̊֐�
[maxvertexcount(3)]
void main(triangle VS_OUT input[3],
	inout TriangleStream<GS_OUT> TriStream) {
	GS_OUT output;

	// �e���_�̌v�Z
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.worldPos = input[i].worldPos;
		output.normal = input[i].normal;

		output.uv = input[i].uv;

		output.posSM = input[i].posSM;
		output.posDLight = input[i].posDLight;
		output.normalDLight = input[i].normalDLight;

		// �o��
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}