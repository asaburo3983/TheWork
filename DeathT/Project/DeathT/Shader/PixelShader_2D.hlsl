Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer ConstantBuffer
{
	float4x4	World;				// ワールド変換行列
	float4x4	View;				// ビュー変換行列
	float4x4	Projection;			// 透視射影変換行列
	float4		CameraPos;			// カメラ座標
	float4		LightVector;		// ライト方向
	float4		LightColor;			// ライトカラー
	float4		MaterialAmbient;	// アンビエント
	float4		MaterialDiffuse;	// ディフューズ
	float4		MaterialSpecular;	// スペキュラー

	matrix SMWorldViewProj;
	float4x4 boneMatrix[50];

	float4 multiplyColor;

	// ワールド×ビュー×透視変換行列(シャドウマップ用)
}

float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0
{


   float4 tex_color = Texture.Sample(TextureSampler, texCoord);
   if (tex_color.a <= 0.0)discard;//a０値非表示処理
   tex_color.x += multiplyColor.x;
   tex_color.y += multiplyColor.y;
   tex_color.z += multiplyColor.z;
   tex_color.w += multiplyColor.w;

   return tex_color;
}