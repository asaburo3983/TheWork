#include "ConstantBuffer.hlsli"


float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float roughness)
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

float3 PointLight(float3 modelWorldPos, float3 lightPos,float lightRange,float3 lightColor) {

    float distance = length(modelWorldPos - lightPos);//頂点との距離を計算
    distance /= lightRange;//範囲内での処理に

    float anser = 1.0f - distance;

    if (anser < 0) {
        anser = 0;
    }
    anser = pow(anser, 2.0f);
    return lightColor * anser;
}
float3 SpotLight(float3 modelWorldPos, float3 lightPos, float lightRange, float3 lightColor,float lightAnglurRange,float3 lightVector) {
    //スポットライトの計算
    lightVector = normalize(lightVector);

    float3 objectVector = normalize(modelWorldPos - lightPos);//オブジェクトのライトへのベクトル

    float scalar = dot(objectVector, lightVector);//ライトとオブジェクトの内積を求める

    float angle = (1.0 - scalar) * 180.0f;//度数が出ます

    float anser = 1.0f - 1.0f / lightAnglurRange * angle;//角度減衰

    float length2 = length(modelWorldPos - lightPos);//距離（実数地）
    length2 /= lightRange;
    anser *= length2;//距離減衰
    if (anser < 0) {
        anser = 0;
    }
    anser = pow(anser, 2.0f);

    return lightColor * anser;
}
float4 main(VS_OUT input) : SV_TARGET{

    float offsetX = 1.5f / 1920.0f;
    float offsetY = 1.5f / 1080.0f;

    float offsetShadowX = 1.0f / 2048.0f*1.5f;
    float offsetShadowY = 1.0f / 2048.0f*1.5f;

     //テクスチャカラー
     float4 tex_color = tex.Sample(smp, input.uv);
     if (tex_color.a <= 0.0)discard;//a０値非表示処理

    float3 normal = normalize(input.normalDLight);
    if (anotherTexture0 == 1) {
        float4 tex_normal = texAnother0.Sample(smp, input.uv);
        normal = normalize(normal * tex_normal.xyz);
    }
    
    float sm0 = texDepth.Sample(smpSM, input.posSM.xy);
    float zShadowBuffar = input.posSM.z;
    float shadowRate = 1.0f;

    //ソフトシャドウ　生成 描画上ほぼ完璧なソフトシャドウ（高負荷）
    float smZ[4][4];
    for (int i = 1; i <= 4; i++) {
        //縦横４ピクセルずつ判定して影をぼかす
         smZ[0][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(offsetShadowX*i, 0.0f));
         smZ[1][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(-offsetShadowX * i, 0.0f));
         smZ[3][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(0.0f, offsetShadowY * i));
         smZ[2][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(0.0f, offsetShadowY * i));
    }

    if (zShadowBuffar > sm0) {
        shadowRate = 0.6;//一番濃い影状態

        float max = 1.0 - shadowRate;
        float weight = max / 16.0f;
        for (int i = 0; i < 4; i++) {
            for (int h = 0; h < 4; h++) {
                if (zShadowBuffar < smZ[i][h]) {
                    shadowRate += weight;
                }
            }
        }
    }

    //一番簡単なシャドウ
    //shadowRate = 1;
    //if (zShadowBuffar > sm0) {
    //    shadowRate = 0.7f;
    //}





    //PBR処理
    float4 albedoColor = tex_color;
    float3 specColor = albedoColor;

    float smooth = roufhness;
    float _metallic = 1.0f - metallic;//反転してる
    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(CameraPos - input.posDLight);

    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -normalize(DLightVector), toEye, smooth);

    float NdotL = saturate(dot(normal, -normalize(DLightVector)));
    float3 lambertDiffuse = DLightColor * NdotL / PI;

    float3 diffuse = diffuseFromFresnel*lambertDiffuse;

    float spec = CookTorranceSpecular(-normalize(DLightVector), toEye, normal, _metallic) * DLightColor;

    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, _metallic);

    float3 lig = 0;
    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    // 滑らかさが高ければ、拡散反射は弱くなる
    lig +=  diffuse * (1.0f - smooth) + spec;
   
    
    float3 pointLight = 0;
    for (int i = 0; i < POINT_LIGHT; i++) {
        if (pointLightRange[i].x > 0)
        pointLight += PointLight(input.worldPos.xyz, pointLightPos[i], pointLightRange[i].x, pointLightColor[i]);
    }
    float3 spotLight = 0;
    for (int i = 0; i < SPOT_LIGHT; i++) {
        if (spotLightRange[i].x > 0)
             spotLight += SpotLight(input.worldPos.xyz, spotLightPos[i], spotLightRange[i].x, spotLightColor[i], spotLightAngularRange[i].x, spotLightTarget[i]);
    }


    // 環境光による底上げ　
    lig += ambientLight.x;
    lig *= albedoColor;

    //ソフトシャドウ追加
    lig *= shadowRate;

    lig += lig * pointLight;// ポイントライトを加算 //先にPBRの理解を完了させるほうがいい
    lig += lig * spotLight;//スポットライトを加算



    float4 finalColor = 1.0f;
    finalColor.xyz = lig;

    finalColor.a = tex_color.a;

    return finalColor;

}