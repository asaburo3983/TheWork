#include "ConstantBuffer.hlsli"




VS_OUT main(VS_IN input) {

	VS_OUT output;

	output.pos = input.pos;
	output.worldPos = 0;
	output.normal = 0;
	output.uv = input.texture_pos;
	output.posSM = 0;
	output.posDLight = 0;
	output.normalDLight = 0;

	//Directionalƒ‰ƒCƒg‚©‚çŒ©‚½’¸“_•ÏŠ·
	output.pos = mul(World, output.pos);
	output.pos = mul(DLightView, output.pos);
	output.pos = mul(DLightProjection, output.pos);
	//output.pos = mul(CameraView, output.pos);
	//output.pos = mul(CameraProjection, output.pos);

	return output;
}
