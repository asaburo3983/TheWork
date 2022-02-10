#include "ConstantBuffer.hlsli"


VS_OUT main(VS_IN input) {

	VS_OUT output;
	float4 pos;

	output.posDLight = 0;
	output.normalDLight = 0;

	pos = input.pos;


	//頂点の座標返還
	output.pos = mul(World, pos);
	output.worldPos = 0;

	output.pos = mul(CameraView, output.pos);
	output.pos = mul(CameraProjection, output.pos);



	output.normal = 0;//法線にもワールド変換を行う

	output.uv = input.texture_pos;


	//ディレクショナルライトから見た頂点
	float4 pos4;

	output.posSM = 0;



	return output;
}
