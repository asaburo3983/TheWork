#include "ConstantBuffer.hlsli"


VS_OUT main(VS_IN input) {

	VS_OUT output;
	float4 pos;

	output.posDLight = 0;
	output.normalDLight = 0;

	pos = input.pos;


	//���_�̍��W�Ԋ�
	output.pos = mul(World, pos);
	output.worldPos = 0;

	output.pos = mul(CameraView, output.pos);
	output.pos = mul(CameraProjection, output.pos);



	output.normal = 0;//�@���ɂ����[���h�ϊ����s��

	output.uv = input.texture_pos;


	//�f�B���N�V���i�����C�g���猩�����_
	float4 pos4;

	output.posSM = 0;



	return output;
}
