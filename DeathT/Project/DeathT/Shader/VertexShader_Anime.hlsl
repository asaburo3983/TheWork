struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 color : COLOR0;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	int4 boneIndex :  BONEINDEX;
	float4 boneWeight : BONEWEIGHT;
};

struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)

};

cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	float4		CameraPos;			// �J�������W
	float4		LightVector;		// ���C�g����
	float4		LightColor;			// ���C�g�J���[
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[

	matrix SMWorldViewProj;
	float4x4 boneMatrix[50];

	// ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)
}
GS_IN main(VS_IN input)
{
	GS_IN output;

	////�{�[���ɂ��ړ�
// ���_�u�����h�̏���
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
		if (input.boneWeight[i] > 0.0 && input.boneIndex[i] != 39)
			comb += boneMatrix[input.boneIndex[i]] * input.boneWeight[i];
	}
	// ���[�J�����W * ���[���h���W�ϊ��s��

	//MultNormalize
	input.pos.w = 1.0;

	float4 combX;
	combX[0] = -comb[0][0] * input.pos.x;
	combX[1] = -comb[1][0] * input.pos.x;
	combX[2] = -comb[2][0] * input.pos.x;
	combX[3] = -comb[3][0] * input.pos.x;

	float4 combY;
	combY[0] = comb[0][1] * input.pos.y;
	combY[1] = comb[1][1] * input.pos.y;
	combY[2] = comb[2][1] * input.pos.y;
	combY[3] = comb[3][1] * input.pos.y;

	float4 combZ;
	combZ[0] = comb[0][2] * input.pos.z;
	combZ[1] = comb[1][2] * input.pos.z;
	combZ[2] = comb[2][2] * input.pos.z;
	combZ[3] = comb[3][2] * input.pos.z;

	float4 combW;
	combW[0] = comb[0][3] * input.pos.w;
	combW[1] = comb[1][3] * input.pos.w;
	combW[2] = comb[2][3] * input.pos.w;
	combW[3] = comb[3][3] * input.pos.w;

	output.pos.x = combX[0] + combY[0] + combZ[0] + combW[0];
	output.pos.y = combX[1] + combY[1] + combZ[1] + combW[1];
	output.pos.z = combX[2] + combY[2] + combZ[2] + combW[2];
	output.pos.w = combX[3] + combY[3] + combZ[3] + combW[3];

	output.pos.x /= output.pos.w;
	output.pos.y /= output.pos.w;
	output.pos.z /= output.pos.w;
	output.pos.w /= output.pos.w;

	float4 animeChangePos = output.pos;
	output.pos = mul(output.pos, World);
	// ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, View);
	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, Projection);

	// �V���h�E�}�b�v�p���_���W
	float4 pos4 = input.pos;

	// ���_���W�@���f�����W�n���������W�n(�V���h�E�}�b�v)
	//output.pos= mul(input.pos, SMWorldViewProj);
	pos4.w = 1.0;
	pos4 = mul(pos4, World);
	pos4 = mul(pos4, View);
	pos4 = mul(pos4, Projection);

	pos4 = animeChangePos;
	//���_�J���[
	output.color = input.color;

	// Texture���W�w��
	output.texture_pos = input.texture_pos;

	float4 normal;
	// �ړ����v�Z�ɔ��f�����Ȃ�
	input.nor.w = 0.0;
	float4  animeChangeNor;
	combX[0] = -comb[0][0] * input.nor.x;
	combX[1] = -comb[1][0] * input.nor.x;
	combX[2] = -comb[2][0] * input.nor.x;
	combX[3] = -comb[3][0] * input.nor.x;
	combY[0] = comb[0][1] * input.nor.y;
	combY[1] = comb[1][1] * input.nor.y;
	combY[2] = comb[2][1] * input.nor.y;
	combY[3] = comb[3][1] * input.nor.y;
	combZ[0] = comb[0][2] * input.nor.z;
	combZ[1] = comb[1][2] * input.nor.z;
	combZ[2] = comb[2][2] * input.nor.z;
	combZ[3] = comb[3][2] * input.nor.z;
	combW[0] = comb[0][3] * input.nor.w;
	combW[1] = comb[1][3] * input.nor.w;
	combW[2] = comb[2][3] * input.nor.w;
	combW[3] = comb[3][3] * input.nor.w;
	animeChangeNor.x = combX[0] + combY[0] + combZ[0] + combW[0];
	animeChangeNor.y = combX[1] + combY[1] + combZ[1] + combW[1];
	animeChangeNor.z = combX[2] + combY[2] + combZ[2] + combW[2];
	animeChangeNor.w = combX[3] + combY[3] + combZ[3] + combW[3];

	animeChangeNor.x /= animeChangeNor.w;
	animeChangeNor.y /= animeChangeNor.w;
	animeChangeNor.z /= animeChangeNor.w;
	animeChangeNor.w /= animeChangeNor.w;
	// ���_�̖@���Ƀ��[���h�s����|�����킹��
	// ���[���h���W��ł̖@���̌����ɕϊ�����
	normal = mul(animeChangeNor, World).xyzw;
	normal = normalize(normal);
	// saturate => �����Ŏw�肵���l��0�`1�Ԃł͈̔͂Ɏ��߂�
	// dot => ���όv�Z
	//output.color *= saturate(dot(normal, LightVector));

	//���C�g�̋���������
	float3 light = LightVector - output.pos;
	float  leng = length(light);

	float bright = 45 * dot(normalize(light), normal) / pow(leng, 2);//���C�g�̋����A���C�g�Ɩ@���A����
	output.color *= bright;



	float4x4 mtx = mul(World, SMWorldViewProj);
	pos4 = mul(animeChangePos, mtx);//���C�g���猩�����_���W

	pos4.xyz = pos4.xyz / pos4.w;			//���K��
	output.PosSM.x = (pos4.x + 1.0) / 2.0;	//
	output.PosSM.y = (-pos4.y + 1.0) / 2.0;
	output.PosSM.z = pos4.z;


	return output;
}
