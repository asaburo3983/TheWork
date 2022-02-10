
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
	
};

VS_OUT main(VS_IN input) {

	VS_OUT output;
	float4 pos;
	float4 normal;


	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
		if (input.boneWeight[i] > 0.0 && input.boneIndex[i] != 512)
			comb += mBoneMatrix[input.boneIndex[i]] * input.boneWeight[i];
	}

	input.pos.w = 1.0;
	float4 combX;
	combX[0] = -comb[0][0] * input.pos.x;
	combX[1] = -comb[1][0] * input.pos.x;
	combX[2] = -comb[2][0] * input.pos.x;
	combX[3] = -comb[3][0] * input.pos.x;
	float4 combY;
	combY[0] = comb[0][1] * input.pos.y;
	combY[1] = comb[1][1] * input.pos.y;
	combY[2] = comb[2][1] * input.pos.y;
	combY[3] = comb[3][1] * input.pos.y;
	float4 combZ;
	combZ[0] = comb[0][2] * input.pos.z;
	combZ[1] = comb[1][2] * input.pos.z;
	combZ[2] = comb[2][2] * input.pos.z;
	combZ[3] = comb[3][2] * input.pos.z;
	float4 combW;
	combW[0] = comb[0][3] * input.pos.w;
	combW[1] = comb[1][3] * input.pos.w;
	combW[2] = comb[2][3] * input.pos.w;
	combW[3] = comb[3][3] * input.pos.w;
	pos.x = combX[0] + combY[0] + combZ[0] + combW[0];
	pos.y = combX[1] + combY[1] + combZ[1] + combW[1];
	pos.z = combX[2] + combY[2] + combZ[2] + combW[2];
	pos.w = combX[3] + combY[3] + combZ[3] + combW[3];
	pos.x /=pos.w;
	pos.y /=pos.w;
	pos.z /=pos.w;
	pos.w /=pos.w;	
	
	normal.x = combX[0] + combY[0] + combZ[0] + combW[0];
	normal.y = combX[1] + combY[1] + combZ[1] + combW[1];
	normal.z = combX[2] + combY[2] + combZ[2] + combW[2];
	normal.w = combX[3] + combY[3] + combZ[3] + combW[3];
	normal.x /=normal.w;
	normal.y /=normal.w;
	normal.z /=normal.w;
	normal.w /=normal.w;

	
	output.pos = mul(World, pos);
	float4 posSM = output.pos;//アニメーションとワールド返還したものを影用座標に入れておく

	output.worldPos = output.pos;

	if (shadow == 0) {
		output.pos = mul(CameraView, output.pos);
		output.pos = mul(CameraProjection, output.pos);//シェーダでは列優先なので注意
	}
	else {
		output.pos = mul(DLightView, output.pos);
		output.pos = mul(DLightProjection, output.pos);//シェーダでは列優先なので注意
	}


	output.normal = mul(World, normal);

	output.uv = input.texture_pos;


	posSM.w = 1;
	posSM = mul(DLightView, posSM);
	posSM = mul(DLightProjection, posSM);//シェーダでは列優先なので注意

	posSM.xyz = posSM.xyz / posSM.w;			//正規化
	output.posSM.x = (posSM.x + 1.0) / 2.0;	//
	output.posSM.y = (-posSM.y + 1.0) / 2.0;
	output.posSM.z = posSM.z;

	return output;
}
