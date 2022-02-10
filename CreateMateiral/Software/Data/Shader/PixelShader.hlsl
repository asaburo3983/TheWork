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

//float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float roughness)
//{
//    // step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
//    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
//    float3 H = normalize(L + V);
//
//    // 粗さは0.5で固定。
//    float energyBias = lerp(0.0f, 1.0f, roughness);
//    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);
//
//    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
//    float dotLH = saturate(dot(L, H));
//
//    // 光源に向かうベクトルとハーフベクトル、
//    // 光が平行に入射したときの拡散反射量を求めている
//    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
//
//    // 法線と光源に向かうベクトルwを利用して拡散反射率を求める
//    float dotNL = saturate(dot(N, L));
//    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));
//
//    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
//    float dotNV = saturate(dot(N, V));
//    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));
//
//    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
//    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
//    return (FL * FV * energyFactor);
//}
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-4 フレネル反射を考慮した拡散反射光を求める

    // 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNL = saturate(dot(N, L));

    // 法線と視線に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNV = saturate(dot(N, V));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (dotNL * dotNV);
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

    float3 distance = abs(modelWorldPos - lightPos);//頂点との距離を計算
    distance /= lightRange;//範囲内での処理に

    float3 anser = float3(1.0f, 1.0f, 1.0f) - distance;
    //TODO anserをfloat型に変えたい
    anser.x = anser.x * anser.y * anser.z;
    if (anser.x < 0) {
        anser.x = 0;
    }
    anser.x = pow(anser.x, 2.0f);
    anser = lightColor * anser.x;
    return anser;
}
float3 SpotLight(float3 modelWorldPos, float3 lightPos, float lightRange, float3 lightColor,float lightAnglurRange,float3 lightVector) {
    //スポットライトの計算
    lightVector = normalize(lightVector);

    float3 objectVector = normalize(modelWorldPos - lightPos);
    float3 length = abs(modelWorldPos - lightPos);

    float scalar = dot(objectVector, lightVector);//内積を求める

    float angle = (1.0 - scalar) * 180.0f;//度数が出ます

    float anser = 1.0f - 1.0f / lightAnglurRange * angle;//角度減衰

    anser *= 1.0f - 1.0f / lightRange * length;//距離減衰
    if (anser < 0) {
        anser = 0;
    }
    anser = pow(anser, 2.0f);

    return lightColor * anser;
}
float4 main(VS_OUT input) : SV_TARGET{

    float offsetX = 1.0f / 1920.0f;
    float offsetY = 1.0f / 1080.0f;

     //テクスチャカラー
     float4 tex_color = tex.Sample(smp, input.uv);
     if (tex_color.a <= 0.0)discard;//a０値非表示処理

     float3 normal = input.normalDLight;
     if (anotherTexture0 == 1) {
         float4 tex_normal = texAnother0.Sample(smp, input.uv);
         normal = normalize(normal * tex_normal.xyz);
        // return(texAnother0.Sample(smp, input.uv));
     }


 
    //normal = mul(CameraView, normal);
    //normal = mul(CameraProjection, normal);

    //ソフトシャドウ　生成
    float sm0 = texDepth.Sample(smpSM, input.posSM.xy);
    float sm1 = texDepth.Sample(smpSM, input.posSM.xy + float2(offsetX, 0.0f));
    float sm2 = texDepth.Sample(smpSM, input.posSM.xy + float2(offsetX, offsetY));
    float sm3 = texDepth.Sample(smpSM, input.posSM.xy + float2(0.0f, offsetY));

    float shadowRate = 0.0f;

    float zShadoBuffar = input.posSM.z;
    if (zShadoBuffar < sm0) {
        shadowRate += 1.0f;
    }
    if (zShadoBuffar < sm1) {
        shadowRate += 1.0f;
    }
    if (zShadoBuffar < sm2) {
        shadowRate += 1.0f;
    }
    if (zShadoBuffar < sm3) {
        shadowRate += 1.0f;
    }
    shadowRate /= 4.0f;

    if (shadowRate == 0.0f) {
        shadowRate += 0.25f;//最低の黒さ
    }
    shadowRate = 0.5f;
    if (zShadoBuffar < sm0) {
        shadowRate = 1.0f;
    }

    //影をテクスチャにする処理  
    if (anotherTexture2 == 1) {
        float4 tex_shadow = 1;
        tex_shadow = texAnother2.Sample(smp, input.uv);
        tex_color = lerp(tex_color, tex_shadow, 1.0f - shadowRate);

    }


    //アウトライン処理(シェーダー部での実装案,2パス方式のほうが精度がよいので考える)
    float outlineCos = dot(normalize(CameraPos.xyz), normalize(normal));
    float ans = outlineCos * 180.0f / 3.141592f;

    float outlineAngle= lerp(0.0f, 90.0f, outline);
    if (ans < outlineAngle && outlineAngle > 0.1) {

        return float4(outlineColorR,outlineColorG,outlineColorB, 1);
    }
    ////リムライト
    //if (ans <outlineAngle+10.0f) {

    //    return float4(0.9, 0.9, 0.9, 1);
    //}

    //PBR処理

    float4 albedoColor = tex_color;
    float3 specColor = albedoColor;
    float smooth = roufhness;
    float _metallic = 1.0f - metallic;//反転してる

    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(CameraPos - input.posDLight);

    //拡散反射（素の色）を求める
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -DLightVector, toEye/*, smooth*/);
    //ライトとモデル法線の内積を出す
    float NdotL = saturate(dot(normal, -DLightVector));
    float3 lambertDiffuse = DLightColor * NdotL / PI;//正規化ランバートの作成　ランバートをPIでわるだけ

    float3 diffuse = diffuseFromFresnel  *lambertDiffuse;

    //鏡面反射率の計算
    float spec = CookTorranceSpecular(-DLightVector, toEye, normal, _metallic) * DLightColor;
    float3 met = float3(1.0f,1.0f,1.0f);
    spec *= lerp(met, specColor, _metallic);

    float3 lig = 0;
    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    // 滑らかさが高ければ、拡散反射は弱くなる
    lig += diffuse * (1.0f - smooth)+spec;
   
    
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
   // return float4(spotLight, 1);//Spotライトの仮出力処理
   // return float4(pointLight, 1);//Pointライトの仮出力仮処理

    // 環境光による底上げ　
    lig += 0.5;
    lig *= albedoColor;

    //ソフトシャドウ追加
    lig *= shadowRate;

    lig += lig * pointLight;// ポイントライトを加算 //先にPBRの理解を完了させるほうがいい
    lig += lig * spotLight;//スポットライトを加算

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    //トゥーン処理
    if (anotherTexture1 == 1) {
        finalColor = tex_color;

        float4 tex_toon = texAnother1.Sample(smp, float2(1.0f-lig.r, 0.5));
        finalColor.xyz *= tex_toon;
    }
    //鏡面反射
    //加算
    float sphereMapU = normal.x / 2 + 0.5f;
    float sphereMapV = normal.y / 2 + 0.5f;
    if (anotherTexture4 == 1) {
        float3 sphereMap = texAnother4.Sample(smp, float2(sphereMapU, sphereMapV)).xyz;
        finalColor.xyz += sphereMap * ((1.0f - roufhness) / 2.0f + metallic / 2.0f);
    }  
    //乗算
    if (anotherTexture5 == 1) {
        float3 sphereMap = texAnother5.Sample(smp, float2(sphereMapU, sphereMapV)).xyz;
        finalColor.xyz *= sphereMap * ((1.0f - roufhness) / 2.0f + metallic / 2.0f);
    }    

    finalColor.a = tex_color.a;

    finalColor.a *= workSpace;//Material作成処理で必要

    return finalColor;

}