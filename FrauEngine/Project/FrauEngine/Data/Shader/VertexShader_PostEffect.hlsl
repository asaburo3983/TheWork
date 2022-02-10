#include "ConstantBuffer_PostEffect.hlsli"

PeraType main(VS_IN input) {
	PeraType output;
	output.pos = input.pos;
	output.uv = input.uv;
	return output;
}
