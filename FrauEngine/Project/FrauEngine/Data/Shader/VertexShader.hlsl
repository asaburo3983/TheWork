#include "ConstantBuffer.hlsli"


VS_OUT main(VS_IN input) {

	VS_OUT output;

	//�f�B���N�V���i�����C�g�̏��
	float4 tmp = input.pos;
	tmp.w = 0.0f;
	output.posDLight = mul(World, tmp);
	tmp = input.normal;
	tmp.w = 0.0f;
	output.normalDLight = mul(World, tmp);

	float4 pos;
	float4 normal;
	pos = input.pos;
	normal = input.normal;


	//���_�̍��W�Ԋ�
	output.pos = mul(World, pos);
	output.worldPos = output.pos;

	output.pos = mul(CameraView, output.pos);
	output.pos = mul(CameraProjection, output.pos);

	

	output.normal = normalize(mul(World, normal));//�@���ɂ����[���h�ϊ����s��

	output.uv = input.texture_pos;


	//�f�B���N�V���i�����C�g���猩�����_
	float4 pos4;
	pos4 = input.pos;
	pos4.w = 1;
	pos4 = mul(World, pos4);
	pos4 = mul(DLightView, pos4);
	pos4 = mul(DLightProjection, pos4);
	//�V���h�E�}�b�v�p�̐[�x�𐳋K��
	pos4.xyz = pos4.xyz / pos4.w;
	output.posSM.x = (pos4.x + 1.0) / 2.0;
	output.posSM.y = (-pos4.y + 1.0) / 2.0;
	output.posSM.z = pos4.z;


	return output;
}
