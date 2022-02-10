#include "ConstantBuffer.hlsli"


VS_OUT main(VS_IN input) {

	VS_OUT output;

	float4 pos;
	pos = input.pos;

	//ボーンアニメーション処理
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
	pos.x /= pos.w;
	pos.y /= pos.w;
	pos.z /= pos.w;
	pos.w /= pos.w;

	//頂点の座標返還
	output.worldPos = 0;
	output.normal = 0;
	output.uv = input.texture_pos;
	output.posSM = 0;
	output.posDLight = 0;
	output.normalDLight = 0;

	//Directionalライトから見た頂点変換
	output.pos = mul(World, pos);
	output.pos = mul(DLightView, output.pos);
	output.pos = mul(DLightProjection, output.pos);


	return output;
}
