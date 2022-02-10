
static const float PI = 3.1415926f;         // ��
static const int POINT_LIGHT = 4;      
static const int SPOT_LIGHT = 4;       


Texture2D<float4> tex:register(t7);
Texture2D<float> texDepth:register(t8);

Texture2D<float4> texAnother0:register(t9);
Texture2D<float4> texAnother1:register(t10);
Texture2D<float4> texAnother2:register(t11);
Texture2D<float4> texAnother3:register(t12);
Texture2D<float4> texAnother4:register(t13);
Texture2D<float4> texAnother5:register(t14);
Texture2D<float4> texAnother6:register(t15);
Texture2D<float4> texAnother7:register(t16);


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
    float4 CameraPos;
}
cbuffer DirectionalLightBuffer : register(b4) {
    float4 ambientLight;
    ///////////////////////
    matrix DLightView;
    matrix DLightProjection;
    float4 DLightPos;
    float4 DLightVector;
    float4 DLightColor;
    /////////////////////////
    float4 pointLightPos[POINT_LIGHT];
    float4 pointLightRange[POINT_LIGHT];//�p�b�L���O�̉e����float4��x�����g�p
    float4 pointLightColor[POINT_LIGHT];
    /////////////////////////
    float4 spotLightPos[SPOT_LIGHT];
    float4 spotLightRange[SPOT_LIGHT];
    float4 spotLightTarget[SPOT_LIGHT];
    float4 spotLightAngularRange[SPOT_LIGHT];
    float4 spotLightColor[SPOT_LIGHT];

}
cbuffer UtilityBuffer : register(b5) {
    int time;
    int meshNum;

}
cbuffer PBRBuffer : register(b6) {


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

    float outline;
    float outlineColorR;
    float outlineColorG;
    float outlineColorB;

    float workSpace;

    //�z��ɂ����CBuffer��n���ۂɃI�t�Z�b�g�̊֌W�œn���Ȃ��Ȃ�
    int anotherTexture0;
    int anotherTexture1;
    int anotherTexture2;
    int anotherTexture3;
    int anotherTexture4;
    int anotherTexture5;
    int anotherTexture6;
    int anotherTexture7;
}

struct VS_IN {
    float4 pos : POSITION0;
    float4 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texture_pos : TEXTURE0;
    int4 boneIndex :  BONEINDEX;
    float4 boneWeight : BONEWEIGHT;

};
struct VS_OUT {
    float4 pos : SV_POSITION;//�V�X�e���p���_���W
    float4 worldPos : POSITION;
    float4 normal : NORMAL;//�@���x�N�g��
    float2 uv  : TEXCOORD;//UV�l
    float3 posSM : POSITION_SM;//�V�X�e���p���_���W

    float4 posDLight : POSITION1;
    float4 normalDLight : NORMAL1;
};