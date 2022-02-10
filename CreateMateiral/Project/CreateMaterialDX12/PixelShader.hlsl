
static const float PI = 3.1415926f;         // π

Texture2D<float4> tex:register(t7);
Texture2D<float> texDepth:register(t8);

Texture2D<float4> texBmp:register(t9);
Texture2D<float4> texAnother0:register(t10);
Texture2D<float4> texAnother1:register(t11);
Texture2D<float4> texAnother2:register(t12);

SamplerState smp:register(s0);//0番スロットに設定されたサンプラ
SamplerState smpSM:register(s1);//0番スロットに設定されたサンプラ


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
    float3 CameraPos;
}
cbuffer DirectionalLightBuffer : register(b4) {
    matrix DLightView;
    matrix DLightProjection;
    float3 DLightPos;

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

struct VS_OUT {
    float4 pos : SV_POSITION;//システム用頂点座標
    float4 worldPos : POSITION;
    float4 normal : NORMAL;//法線ベクトル
    float2 uv  : TEXCOORD;//UV値
    float3 posSM : POSITION_SM;//システム用頂点座標

    float4 posIN : POSITION1;
    float4 normalIN : NORMAL1;
};

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}
// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}
/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
///
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V ,float roughness)
{
    // step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 粗さは0.5で固定。


    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(L, H));

    // 光源に向かうベクトルとハーフベクトル、
    // 光が平行に入射したときの拡散反射量を求めている
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // 法線と光源に向かうベクトルwを利用して拡散反射率を求める
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (FL * FV * energyFactor);
}
/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float _metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = _metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}



float4 main(VS_OUT input) : SV_TARGET{
   // return float4(1,0,0,1);

    //テクスチャカラー
    float4 tex_color = tex.Sample(smp, input.uv);


    if (anotherTexture0) {
        tex_color = texAnother0.Sample(smp, input.uv);
    }
    if (anotherTexture1) {
        tex_color = texAnother1.Sample(smp, input.uv);
    }
    if (anotherTexture2) {
        tex_color = texAnother2.Sample(smp, input.uv);
    }


    if (tex_color.a <= 0.0)discard;//a０値非表示処理

    float3 normal = input.normalIN;
  
    if (bmpTexture) {
        normal = texBmp.Sample(smp, input.uv);
        normal = mul(World, normal);//法線にもワールド変換を行う

    }
     //シャドウマップ
    float sm = texDepth.Sample(smpSM, input.posSM.xy);
    float sma = (input.posSM.z < sm) ? 1.0 : 0.5;

   
    



    //step2
    float4 albedoColor = tex_color;
    float3 specColor = albedoColor;

    float smooth = (1.0f - roufhness);//反転して３倍してる　距離の関係もあるから３倍はしなくてもいいかも
    float _metallic = 1.0f - metallic;//反転してる
    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(CameraPos - input.posIN);

    float3 lig = 0;
    //step3
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -DLightVector, toEye, smooth);

    float NdotL = saturate(dot(normal, -DLightVector));
    float3 lambertDiffuse = DLightColor * NdotL / PI;

    float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

    //step5
    float spec = CookTorranceSpecular(-DLightVector, toEye, normal, _metallic) * DLightColor;

    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, _metallic);

    // step-6 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    // 滑らかさが高ければ、拡散反射は弱くなる
    lig += diffuse * (1.0f - smooth) + spec;

    // 環境光による底上げ
    lig += 1 * albedoColor;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;

    //finalColor.xyz *= sma;
    finalColor.a *= clearcoatGloss;
    return finalColor;

}