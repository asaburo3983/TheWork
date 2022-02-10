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

	output.pos = mul(input.pos, World);
	// ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, View);
	float4 posView = output.pos;
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


	//���_�J���[
	output.color = input.color;

	// Texture���W�w��
	output.texture_pos = input.texture_pos;

	float4 normal;
	// �ړ����v�Z�ɔ��f�����Ȃ�
	input.nor.w = 0.0;
	// ���_�̖@���Ƀ��[���h�s����|�����킹��
	// ���[���h���W��ł̖@���̌����ɕϊ�����
	normal = mul(input.nor, World).xyzw;
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
	pos4 = mul(input.pos, mtx);//���C�g���猩�����_���W

	pos4.xyz = pos4.xyz / pos4.w;			//���K��
	output.PosSM.x = (pos4.x + 1.0) / 2.0;	//
	output.PosSM.y = (-pos4.y + 1.0) / 2.0;
	output.PosSM.z = pos4.z;


	return output;
}
