struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;

	// 追加
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // 頂点座標(シャドウマップの透視座標系)
};


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
Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    TextureSM : register(t0[1]);

//Texture2D    TextureSM3 : register(t0[3]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM4 : register(t0[4]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM5 : register(t0[5]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM6 : register(t0[6]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM7 : register(t0[7]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM8 : register(t0[8]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM9 : register(t0[9]); // Textureをスロット0の0番目のテクスチャレジスタに設定
//Texture2D    TextureSM10 : register(t0[10]); // Textureをスロット0の0番目のテクスチャレジスタに設定



SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定
SamplerState SamplerSM : register(s0[1]); // Samplerをスロット0の0番目のサンプラレジスタに設定




float4 main(PS_IN input) : SV_Target
{
	// テクスチャカラーの取得
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);
	//if (tex_color.a <= 0.0)discard;//a０値非表示処理

	// シャドウマップ
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);

	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;



	//マテリアルと光源対応（一番きれい）
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// スペキュラー

	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;
	//float4 specular_color = *specular_factor

	//アンチエイリアスに伴う色のバイアス
	/*tex_color.r = pow(tex_color.r, 2.2);
	tex_color.g = pow(tex_color.g, 2.2);
	tex_color.b = pow(tex_color.b, 2.2);*/

	//return sma;
	tex_color *= multiplyColor;
	tex_color.x *= sma;
	tex_color.y *= sma;
	tex_color.z *= sma;

	//tex_color.a = 1.0f;
	return tex_color;

	//マテリアルと光源対応（一番きれい）布の質感とかも出る
	//return (ambient_color + diffuse_color * tex_color)*sma;

	//光源対応(暗くて汚いので*2している) アニメ調、色がくっきりと出る　質感はない
	return input.color * tex_color * 2;

	//テクスチャカラーのみ(アニメ調)
	return tex_color;

}
