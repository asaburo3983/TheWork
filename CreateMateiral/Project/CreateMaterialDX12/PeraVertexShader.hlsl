

//ƒyƒ‰ƒ|ƒŠƒSƒ“•`‰æ—p
struct PeraType {
	float4 pos: SV_POSITION;
	float2 uv:TEXCOORD;
};

PeraType PeraVS(float4 pos:POSITION, float2 uv : TEXCOORD) {
	PeraType output;
	output.pos = pos;
	output.uv = uv;
	return output;
}
