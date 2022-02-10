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

float SpcFresnel(float f0, float u){

    return f0 + (1 - f0) * pow(1 - u, 5);
}

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-4 �t���l�����˂��l�������g�U���ˌ������߂�

    // �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotNL = saturate(dot(N, L));

    // �@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotNV = saturate(dot(N, V));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (dotNL * dotNV);
}

float CookTorranceSpecular(float3 L, float3 V, float3 N, float _metallic)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = _metallic;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = Beckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = SpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

float3 PointLight(float3 modelWorldPos, float3 lightPos, float lightRange, float3 lightColor) {

    float distance = length(modelWorldPos - lightPos);//���_�Ƃ̋������v�Z
    distance /= lightRange;//�͈͓��ł̏�����

    float anser = 1.0f - distance;

    if (anser < 0) {
        anser = 0;
    }
    anser = pow(anser, 2.0f);
    return lightColor * anser;
}
float3 SpotLight(float3 modelWorldPos, float3 lightPos, float lightRange, float3 lightColor, float lightAnglurRange, float3 lightVector) {
    //�X�|�b�g���C�g�̌v�Z
    lightVector = normalize(lightVector);

    float3 objectVector = normalize(modelWorldPos - lightPos);//�I�u�W�F�N�g�̃��C�g�ւ̃x�N�g��

    float scalar = dot(objectVector, lightVector);//���C�g�ƃI�u�W�F�N�g�̓��ς����߂�

    float angle = (1.0 - scalar) * 180.0f;//�x�����o�܂�

    float anser = 1.0f - 1.0f / lightAnglurRange * angle;//�p�x����

    float length2 = length(modelWorldPos - lightPos);//�����i�����n�j
    length2 /= lightRange;
    anser *= length2;//��������
    if (anser < 0) {
        anser = 0;
    }
    anser = pow(anser, 2.0f);

    return lightColor * anser;
}
float4 main(VS_OUT input) : SV_TARGET{

    float offsetX = 1.5f / 1920.0f;
    float offsetY = 1.5f / 1080.0f;

    float offsetShadowX = 1.0f / 2048.0f * 1.5f;
    float offsetShadowY = 1.0f / 2048.0f * 1.5f;
    //�e�N�X�`���J���[
    float4 tex_color = tex.Sample(smp, input.uv);
    if (tex_color.a <= 0.0)discard;//a�O�l��\������

    float3 normal = normalize(input.normalDLight);
   if (anotherTexture0 == 1) {
       float4 tex_normal = texAnother0.Sample(smp, input.uv);
       normal = normalize(normal * tex_normal.xyz);
    }

   float sm0 = texDepth.Sample(smpSM, input.posSM.xy);
   float zShadowBuffar = input.posSM.z;
   float shadowRate = 1.0f;

   //�\�t�g�V���h�E�@���� �`���قڊ����ȃ\�t�g�V���h�E�i�����ׁj
   float smZ[4][4];
   for (int i = 1; i <= 4; i++) {
       //�c���S�s�N�Z�������肵�ĉe���ڂ���
       smZ[0][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(offsetShadowX * i, 0.0f));
       smZ[1][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(-offsetShadowX * i, 0.0f));
       smZ[3][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(0.0f, offsetShadowY * i));
       smZ[2][i - 1] = texDepth.Sample(smpSM, input.posSM.xy + float2(0.0f, offsetShadowY * i));
   }

   if (zShadowBuffar > sm0) {
       shadowRate = 0.6;//��ԔZ���e���

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

   //PBR����
   float4 albedoColor = tex_color;
   float3 specColor = albedoColor;

   float smooth = roufhness;
   float _metallic = 1.0f - metallic;//���]���Ă�
   //�t���l��&�����o�[�g����f�B�t���[�Y���v�Z
   float3 toEye = normalize(CameraPos - input.posDLight);
   float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -DLightVector, toEye);
   float NdotL = saturate(dot(normal, -DLightVector));
   float3 lambertDiffuse = DLightColor * NdotL / PI;

   float3 diffuse = diffuseFromFresnel * lambertDiffuse;

   //���^���b�N����N�b�N�g�����X�ŃX�y�L�������v�Z
   float spec = CookTorranceSpecular(-DLightVector, toEye, normal, _metallic) * DLightColor;
   spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, _metallic);

   float3 lig = 0;
   lig += diffuse * (1.0f - smooth) + spec;
    //���C�g����
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


   // �����ɂ���グ�@
   lig += ambientLight.x;
   lig *= albedoColor;

    //�V���h�E���Ȃ����}�X�N����
   if (anotherTexture2 == 1) {
       float4 tex_shadowMask = texAnother2.Sample(smp, input.uv);
       //���F�ɋ߂��Ɨ�����e���Ȃ���
       if (tex_shadowMask.r > 0.5) {
           shadowRate = 1;
       }
   }

   //�\�t�g�V���h�E�̓g�D�[�������̌�

   lig += lig * pointLight;// �|�C���g���C�g�����Z
   lig += lig * spotLight;//�X�|�b�g���C�g�����Z


   float4 finalColor = 1.0f;
   finalColor.xyz = lig;
   //�g�D�[������
   if (anotherTexture1 == 1) {
       finalColor = tex_color;
       //finalColor.xyz = lig;

       float toon = 0;
       toon = 1.0f - lig.r;
       if (toon < 0) {
           toon = 0;
       }
       if (toon > 1) {
           toon = 1;
       }

       float4 tex_toon = texAnother1.Sample(smp, float2(toon, 0.5));
       finalColor.xyz *= tex_toon;

   }
   //���ʔ���
   //���Z
   float sphereMapU = normal.x / 2 + 0.5f;
   float sphereMapV = normal.y / 2 + 0.5f;
   if (anotherTexture4 == 1) {
       float3 sphereMap = texAnother4.Sample(smp, float2(sphereMapU, sphereMapV)).xyz;
       finalColor.xyz += sphereMap * ((1.0f - roufhness) / 2.0f + metallic / 2.0f);
   }
   //��Z
   if (anotherTexture5 == 1) {
       float3 sphereMap = texAnother5.Sample(smp, float2(sphereMapU, sphereMapV)).xyz;
       finalColor.xyz *= sphereMap * ((1.0f - roufhness) / 2.0f + metallic / 2.0f);
   }
   //�\�t�g�V���h�E�ǉ�
   finalColor.xyz *= shadowRate;

   finalColor.a = tex_color.a;

   return finalColor;

}