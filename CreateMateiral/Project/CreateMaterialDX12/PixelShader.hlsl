
static const float PI = 3.1415926f;         // ��

Texture2D<float4> tex:register(t7);
Texture2D<float> texDepth:register(t8);

Texture2D<float4> texBmp:register(t9);
Texture2D<float4> texAnother0:register(t10);
Texture2D<float4> texAnother1:register(t11);
Texture2D<float4> texAnother2:register(t12);

SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��
SamplerState smpSM:register(s1);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v��


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
    float4 pos : SV_POSITION;//�V�X�e���p���_���W
    float4 worldPos : POSITION;
    float4 normal : NORMAL;//�@���x�N�g��
    float2 uv  : TEXCOORD;//UV�l
    float3 posSM : POSITION_SM;//�V�X�e���p���_���W

    float4 posIN : POSITION1;
    float4 normalIN : NORMAL1;
};

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}
// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}
/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <remark>
/// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
/// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
/// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
/// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
///
/// </remark>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V ,float roughness)
{
    // step-1 �f�B�Y�j�[�x�[�X�̃t���l�����˂ɂ��g�U���˂�^�ʖڂɎ�������B
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e����0.5�ŌŒ�B


    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(L, H));

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
    // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL * FV * energyFactor);
}
/// <summary>
/// Cook-Torrance���f���̋��ʔ��˂��v�Z
/// </summary>
/// <param name="L">�����Ɍ������x�N�g��</param>
/// <param name="V">���_�Ɍ������x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <param name="metallic">�����x</param>
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



float4 main(VS_OUT input) : SV_TARGET{
   // return float4(1,0,0,1);

    //�e�N�X�`���J���[
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


    if (tex_color.a <= 0.0)discard;//a�O�l��\������

    float3 normal = input.normalIN;
  
    if (bmpTexture) {
        normal = texBmp.Sample(smp, input.uv);
        normal = mul(World, normal);//�@���ɂ����[���h�ϊ����s��

    }
     //�V���h�E�}�b�v
    float sm = texDepth.Sample(smpSM, input.posSM.xy);
    float sma = (input.posSM.z < sm) ? 1.0 : 0.5;

   
    



    //step2
    float4 albedoColor = tex_color;
    float3 specColor = albedoColor;

    float smooth = (1.0f - roufhness);//���]���ĂR�{���Ă�@�����̊֌W�����邩��R�{�͂��Ȃ��Ă���������
    float _metallic = 1.0f - metallic;//���]���Ă�
    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
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

    // step-6 ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
    // ���炩����������΁A�g�U���˂͎キ�Ȃ�
    lig += diffuse * (1.0f - smooth) + spec;

    // �����ɂ���グ
    lig += 1 * albedoColor;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;

    //finalColor.xyz *= sma;
    finalColor.a *= clearcoatGloss;
    return finalColor;

}