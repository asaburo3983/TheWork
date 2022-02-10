
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
	int meshNum;
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
//Bool型は使うな、４バイトでアライメントされて消えるぞ




struct VS_IN {
	float4 pos : POSITION0;
	float4 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texture_pos : TEXTURE0;
	int4 boneIndex :  BONEINDEX;
	float4 boneWeight : BONEWEIGHT;

};
struct VS_OUT {
	float4 pos : SV_POSITION;//システム用頂点座標
	float4 worldPos : POSITION;
	float4 normal : NORMAL;//法線ベクトル
	float2 uv  : TEXCOORD;//UV値
	float3 posSM : POSITION_SM;//システム用頂点座標

	float4 posIN : POSITION1;
	float4 normalIN : NORMAL1;

};

VS_OUT main(VS_IN input) {

	VS_OUT output;//ピクセルシェーダへ渡す値

	//移動を計算しない情報
	float4 tmp = input.pos;
	tmp.w = 0.0f;
	output.posIN = mul(World, tmp);
	tmp = input.normal;
	tmp.w = 0.0f;
	output.normalIN = mul(World, tmp);

	
	float4 pos;
	float4 normal;

	pos = input.pos;
	normal = input.normal;

	output.pos = mul(World, pos);
	output.worldPos = output.pos;
	if (shadow == 0) {
		output.pos = mul(CameraView, output.pos);
		output.pos = mul(CameraProjection, output.pos);//シェーダでは列優先なので注意
	}
	else {
		output.pos = mul(DLightView, output.pos);
		output.pos = mul(DLightProjection, output.pos);//シェーダでは列優先なので注意
	}
	
	output.normal = mul(World, normal);//法線にもワールド変換を行う
	//output.normal = normal;
	output.uv = input.texture_pos;



	float4 pos4;
	pos4 = input.pos;
	pos4.w = 1;
	pos4 = mul(World, pos4);
	pos4 = mul(DLightView, pos4);
	pos4 = mul(DLightProjection, pos4);//シェーダでは列優先なので注意
	

	pos4.xyz = pos4.xyz / pos4.w;			//正規化
	output.posSM.x = (pos4.x + 1.0) / 2.0;	//
	output.posSM.y = (-pos4.y + 1.0) / 2.0;
	output.posSM.z = pos4.z;


	return output;
}
